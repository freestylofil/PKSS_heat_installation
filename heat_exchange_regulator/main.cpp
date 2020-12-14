#include <iostream>
#include "Logger.h"
#include "Simulation.h"
#include "Network.h"
#include "NetworkException.h"

int main()
{
	try
	{
		Network network;
#ifndef NO_TIME
		unsigned long long timeData;
#endif
#ifndef NO_ENERGY_SUPPLIER
		Json energySupplierData;
#endif
#ifndef NO_LOGGER
		Json loggingData;
#endif
#ifndef NO_HEAT_EXCHANGER
		Json heatExchangerData;
#endif
		std::array<Json, 4> buildingData;




#ifndef NO_TIME
		Result timeResult = Result::success;
#endif
#ifndef NO_ENERGY_SUPPLIER
		Result energySupplierResult = Result::success;
#endif
#ifndef NO_LOGGER
		Result loggingResult = Result::success;
#endif
#ifndef NO_HEAT_EXCHANGER
		Result heatExchangerResult = Result::success;
#endif
		std::array<Result, 4> buildingResults{
			Result::success,
			Result::success,
			Result::success,
			Result::success
		};


		const double Tall = 160.0*3600.0;

		unsigned long long prevTime = time(0);
		double dt = 300.0;
		double To = 20.0;
		double Tpco = 20.0;
		double Tm = 20.0;
		double Tzco = 20.0;
		double Tzm = 60.0;
		Simulation simulation;

		while (true)
		{
#ifndef NO_LOGGER
			if (loggingResult != Result::success)
			{
				network.loggingClient.recconectToServer(loggingResult);
			}
#endif
#ifndef NO_TIME
			network.timeClient.requestTime(timeData, timeResult);
#endif
#ifndef NO_ENERGY_SUPPLIER
			if (energySupplierResult == Result::success)
			{
				network.energySupplierClient.requestData(energySupplierData, energySupplierResult);
			}
#endif
#ifndef NO_HEAT_EXCHANGER
			if (heatExchangerResult == Result::success)
			{
				network.heatExchangerClient.requestData(heatExchangerData, heatExchangerResult);
			}
#endif
#if N_BUILDINGS > 0
			for (std::size_t i = 0; i < network.buildingClients.size(); ++i)
			{
				if (buildingResults[i] == Result::success)
				{
					network.buildingClients[i].requestData(buildingData[i], buildingResults[i]);
				}
			}
#endif
			network.joinClients();
#ifndef NO_ENERGY_SUPPLIER
			if (energySupplierResult == Result::connectionReset || energySupplierResult == Result::genericError)
			{
				network.energySupplierClient.recconectToServer(energySupplierResult);
			}
#endif
#ifndef NO_HEAT_EXCHANGER
			if (heatExchangerResult == Result::connectionReset || heatExchangerResult == Result::genericError)
			{
				network.heatExchangerClient.recconectToServer(heatExchangerResult);
			}
#endif
#if N_BUILDINGS > 0
			for (std::size_t i = 0; i < network.buildingClients.size(); ++i)
			{
				if (buildingResults[i] == Result::connectionReset || buildingResults[i] == Result::genericError)
				{
					network.buildingClients[i].recconectToServer(buildingResults[i]);
				}
			}
#endif
#ifndef NO_TIME
			if (timeResult == Result::success)
			{
				dt = double(timeData - prevTime);
				prevTime = timeData;
			}
#endif
#ifndef NO_ENERGY_SUPPLIER
			if (!energySupplierData.empty())
			{
				To = double(energySupplierData["To"]);
				Tzm = double(energySupplierData["Tzm"]);
			}
#endif
#ifndef NO_HEAT_EXCHANGER
			if (!heatExchangerData.empty())
			{
				Tzco = double(heatExchangerData["Tzco"]);
			}
#endif
#if N_BUILDINGS > 0
			double Tleft = Tall;
			Tpco = 0.0;
			for (std::size_t i = 0; i < network.buildingClients.size(); ++i)
			{
				if (!buildingData[i].empty())
				{
					Tpco += double(buildingData[i]["Tpcob"]) * double(buildingData[i]["Fcob"]) / Tall;
					Tleft -= double(buildingData[i]["Tpcob"]);
				}
			}
			Tpco += Tleft * Tzco;
#endif
			Simulation::OutputData out = simulation.step({ dt, To, Tm, Tzco });
			Json dataToSend;
			dataToSend["Fzm"] = out.Fzm;
			dataToSend["Fzco"] = out.Tzm;
			dataToSend["Tpco"] = Tpco;
#ifndef NO_SERVER
			network.server.bindData(dataToSend);
#endif
#ifndef NO_LOGGER
			network.loggingClient.join();
			if (loggingResult == Result::success)
			{
				network.loggingClient.logDataToServer(dataToSend, timeData, loggingResult);
			}
#endif
			LOG << "\n\n\n";

			Sleep(1000);
			network.joinClients();
		}
	}
	catch (const NetworkException& e)
	{
		MessageBox(nullptr, e.what(), e.getType(), MB_OK | MB_ICONEXCLAMATION);
		return 1;
	}
	catch (const std::exception e)
	{
		MessageBox(nullptr, e.what(), "Standar Exception", MB_OK | MB_ICONEXCLAMATION);
		return 1;
	}
	catch (...)
	{
		MessageBox(nullptr, "No details avalible", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
		return 1;
	}
	return 0;
}