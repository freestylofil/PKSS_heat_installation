#include "Client.h"
#include "Logger.h"
#include <chrono>

Client::Client(IPEndpoint serverEndpoint, TransmissionType transmissionType, unsigned long timeout)
	: serverEndpoint(serverEndpoint), transmissionType(transmissionType), timeout(timeout)
{
}

void Client::connectToServer()
{
	assert(transmissionType == TransmissionType::unicast);
	socket.create(transmissionType, timeout);
	assert(!clientThread);
	clientThread = std::make_unique<std::thread>(&Client::establishConnection, this, serverEndpoint);
}

void Client::recconectToServer(Result& result)
{
	assert(transmissionType == TransmissionType::unicast);
	socket.close();
	socket.create(transmissionType, timeout);
	assert(!clientThread);
	clientThread = std::make_unique<std::thread>(&Client::reestablishConnection, this, socket.getServerEndpoint(), std::ref(result));
}

void Client::bind(IPEndpoint endpoint)
{
	assert(transmissionType == TransmissionType::broadcast);
	socket.create(transmissionType, timeout);
	socket.bind(endpoint);
}

void Client::requestData(Json& recievedData, Result& result)
{
	assert(!clientThread);
	clientThread = std::make_unique<std::thread>(&Client::requestDataFromSerever, this, std::ref(recievedData), std::ref(result));
}

void Client::recieveData(Json& recievedData, Result& result)
{
	assert(!clientThread);
	clientThread = std::make_unique<std::thread>(&Client::recieveDataT, this, std::ref(recievedData), std::ref(result));
}

void Client::requestTime(unsigned long long& time, Result& result)
{
	assert(!clientThread);
	clientThread = std::make_unique<std::thread>(&Client::requestTimeFromSerever, this, std::ref(time), std::ref(result));
}

void Client::logDataToServer(Json& data, long long time, Result& result)
{
	assert(!clientThread);
	clientThread = std::make_unique<std::thread>(&Client::logData, this, data, time, std::ref(result));
}

void Client::join()
{
	if (clientThread)
	{
		clientThread->join();
		clientThread.reset();
	}
}

TransmissionType Client::getTransmissionType() const
{
	return transmissionType;
}

Client::~Client()
{
	alive.store(false);
	join();
}

void Client::establishConnection(const IPEndpoint& serverEndpoint)
{
	LOG << "trying to connect to "  + serverEndpoint.toString() + '\n';
	while (alive.load() && socket.connect(serverEndpoint) != Result::success);

	if (alive)
	{
		LOG << "connected to " + serverEndpoint.toString() + '\n';
	}
}

void Client::reestablishConnection(const IPEndpoint& serverEndpoint, Result& result)
{
	std::chrono::steady_clock::time_point entranceTime = std::chrono::steady_clock::now();
	LOG << "trying to connect to " + serverEndpoint.toString() + '\n';
	while (alive.load() && (result = socket.connect(serverEndpoint)) != Result::success)
	{
		std::chrono::steady_clock::time_point currTime = std::chrono::steady_clock::now();
		std::chrono::steady_clock::duration duration = currTime - entranceTime;
		if (duration.count() > timeout)
		{
			LOG << "could not reconnect to server " + serverEndpoint.toString() + " - timeout\n";
			result = Result::connectionReset;
			break;
		}
	}

	if (alive)
	{
		LOG << "connected to " + serverEndpoint.toString() + '\n';
	}
}

void Client::requestDataFromSerever(Json& dataRecieved, Result& result)
{
	
	LOG << "requesting data from " + serverEndpoint.toString() + '\n';
	dataRecieved.clear();
	char data = 0;
	int bytesSend = 0;
	if (transmissionType == TransmissionType::unicast)
	{
		result = socket.send(reinterpret_cast<const void*>(&data), sizeof(data), bytesSend);
		if (result == Result::success)
		{
			std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
			std::chrono::steady_clock::time_point currTime = std::chrono::steady_clock::now();
			std::chrono::duration<double> duration = currTime - startTime;
			long long timeElapsed = long long(1000.0 * duration.count());
			while ((result = socket.recieveJson(dataRecieved)) == Result::wouldBlock && timeElapsed <= timeout)
			{
				currTime = std::chrono::steady_clock::now();
				duration = currTime - startTime;
				timeElapsed = long long(1000.0 * duration.count());
			}
			if (timeElapsed > timeout)
			{
				LOG << "could not recieve data from server " + serverEndpoint.toString() + " - timeout\n";
				result = Result::success;
			}
			else if (result == Result::success)
			{
				LOG << "recieved data from " + serverEndpoint.toString() + " data recieved: " + dataRecieved.dump() + '\n';
			}
			else
			{
				LOG << "could not recieve data from server " + serverEndpoint.toString() + " - generic error\n";
			}
		}
		else
		{
			LOG << "could not recieved data from " + socket.toString() + '\n';
		}
	}
	else
	{
		result = socket.sendTo(reinterpret_cast<const void*>(&data), sizeof(data), bytesSend, serverEndpoint);
		if (result == Result::success)
		{
			std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
			std::chrono::steady_clock::time_point currTime = std::chrono::steady_clock::now();
			std::chrono::duration<double> duration = currTime - startTime;
			long long timeElapsed = long long(1000.0 * duration.count());
			while ((result = socket.recieveJson(dataRecieved)) == Result::wouldBlock && timeElapsed <= timeout)
			{
				currTime = std::chrono::steady_clock::now();
				duration = currTime - startTime;
				timeElapsed = long long(1000.0 * duration.count());
			}
			if (timeElapsed > timeout)
			{
				LOG << "could not recieve data from server " + serverEndpoint.toString() + " - timeout\n";
				result = Result::success;
			}
			else if (result == Result::success)
			{
				LOG << "recieved data from " + serverEndpoint.toString() + " data recieved: " + dataRecieved.dump() + '\n';
			}
			else
			{
				LOG << "could not recieve data from server " + serverEndpoint.toString() + " - generic error\n";
			}
		}
		else
		{
			LOG << "could not recieved data from " + socket.toString() + '\n';
		}
	}
}

void Client::requestTimeFromSerever(unsigned long long& time, Result& result)
{
	LOG << "requesting data from " + serverEndpoint.toString() + '\n';
	char data = 0;
	int bytesSend = 0;
	if (transmissionType == TransmissionType::unicast)
	{
		result = socket.send(reinterpret_cast<const void*>(&data), sizeof(data), bytesSend);
		if (result == Result::success)
		{
			std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
			std::chrono::steady_clock::time_point currTime = std::chrono::steady_clock::now();
			std::chrono::duration<double> duration = currTime - startTime;
			long long timeElapsed = long long(1000.0 * duration.count());
			while ((result = socket.recieveTime(time)) == Result::wouldBlock && timeElapsed <= timeout)
			{
				currTime = std::chrono::steady_clock::now();
				duration = currTime - startTime;
				timeElapsed = long long(1000.0 * duration.count());
			}
			if (timeElapsed > timeout)
			{
				LOG << "could not recieve data from server " + serverEndpoint.toString() + " - timeout\n";
				result = Result::success;
			}
			else if (result == Result::success)
			{
				LOG << "recieved data from " + serverEndpoint.toString() + " data recieved: " + std::to_string(time) + '\n';
			}
			else
			{
				LOG << "could not recieve data from server " + serverEndpoint.toString() + " - generic error\n";
			}
		}
		else
		{
			LOG << "could not recieved data from " + socket.toString() + '\n';
		}
	}
	else
	{
		result = socket.sendTo(reinterpret_cast<const void*>(&data), sizeof(data), bytesSend, serverEndpoint);
		if (result == Result::success)
		{
			std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();
			std::chrono::steady_clock::time_point currTime = std::chrono::steady_clock::now();
			std::chrono::duration<double> duration = currTime - startTime;
			long long timeElapsed = long long(1000.0 * duration.count());
			while ((result = socket.recieveTime(time)) == Result::wouldBlock && timeElapsed <= timeout)
			{
				currTime = std::chrono::steady_clock::now();
				duration = currTime - startTime;
				timeElapsed = long long(1000.0 * duration.count());
			}
			if (timeElapsed > timeout)
			{
				LOG << "could not recieve data from server " + serverEndpoint.toString() + " - timeout\n";
				result = Result::success;
			}
			else if (result == Result::success)
			{
				LOG << "recieved data from " + serverEndpoint.toString() + " data recieved: " + std::to_string(time) + '\n';
			}
			else
			{
				LOG << "could not recieve data from server " + serverEndpoint.toString() + " - generic error\n";
			}
		}
		else
		{
			LOG << "could not recieved data from " + socket.toString() + '\n';
		}
	}
}

void Client::recieveDataT(Json& recievedData, Result& result)
{
	result = socket.recieveJson(recievedData);
}

void Client::logData(Json data, long long time, Result& result)
{
	for (const auto& iteam : data.items())
	{
		Json log;
		log["request"] = "PUT";
		log["variable"] = iteam.key();
		log["timestamp"] = time;
		log["value"] = iteam.value();
		result = socket.sendJson(log);
		if (result == Result::connectionReset)
		{
			return;
		}
	}
}
