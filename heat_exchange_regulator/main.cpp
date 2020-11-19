#include <iostream>
#include <Logger.h>
#include "Simulation.h"
#include "Network.h"

int main()
{
	int id = 0;
	Network network;
	Simulation simulation;

#ifndef NO_TIMEENDPOINT
	Json timeData;
#endif
#ifndef NO_ENERGY_SUPPLIER
	Json energySupplierData;
#endif
#ifndef NO_HEAT_EXCHANGER
	Json heatExchangerEarrayData;
#endif
	std::array<Json, 4> buildingData;

#ifndef NO_TIMEENDPOINT
	Result timeResult;
#endif
#ifndef NO_ENERGY_SUPPLIER
	Result energySupplierResult;
#endif
#ifndef NO_HEAT_EXCHANGER
	Result heatExchangerEarrayResult;
#endif
	std::array<Result, 4> buildingDataRequestResults;

#ifndef NO_TIMEENDPOINT
	network.timeClient.recieveData(timeData, timeResult);
	network.timeClient.join();
	while (timeResult == Result::Success)
	{
#else
	while(true)
	{
		Sleep(1000);
#endif
#ifndef NO_ENERGY_SUPPLIER
		network.energySupplierClient.requestData(energySupplierData, energySupplierResult);
#endif
#ifndef NO_HEAT_EXCHANGER
		network.heatExchangerClient.requestData(heatExchangerEarrayData, heatExchangerEarrayResult);
#endif
		for (std::size_t i = 0; i < network.buildingClients.size(); ++i)
		{
			network.buildingClients[i].requestData(buildingData[i], buildingDataRequestResults[i]);
		}
		network.joinUnicastClients();

#ifndef NO_ENERGY_SUPPLIER
		if (energySupplierResult == Result::ConnectionReset)
		{
			network.energySupplierClient.recconectToServer();
		}
#endif
#ifndef NO_HEAT_EXCHANGER
		if (heatExchangerEarrayResult == Result::ConnectionReset)
		{
			network.heatExchangerClient.recconectToServer();
		}
#endif
		for (std::size_t i = 0; i < network.buildingClients.size(); ++i)
		{
			if (buildingDataRequestResults[i] == Result::ConnectionReset)
			{
				network.buildingClients[i].requestData(buildingData[i], buildingDataRequestResults[i]);
			}
		}
		network.joinUnicastClients();

		Json dataToSend;
		dataToSend["id"] = id++;
		network.server.bindData(dataToSend);
		LOG << "\n\n\n";
		getchar();

#ifndef NO_TIMEENDPOINT
		network.timeClient.recieveData(timeData, timeResult);
		network.timeClient.join();
#endif
	}
}
//Fzm, Tpco