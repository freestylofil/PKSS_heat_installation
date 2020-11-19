#include "Client.h"
#include "Logger.h"

void Client::connectToServer(IPEndpoint serverEndpoint)
{
	if (socket.create() != Result::Success)
	{
		throw std::runtime_error("Couldnt create socket on " + serverEndpoint.toString());
	}
	if (!clientThread)
	{
		clientThread = std::make_unique<std::thread>(&Client::establishConnection, this, serverEndpoint);
	}
}

void Client::bindToServer(IPEndpoint serverEndpoint)
{
	if (socket.createBroadcast() != Result::Success)
	{
		throw std::runtime_error("Couldnt create socket on " + serverEndpoint.toString());
	}
	socket.bind(serverEndpoint);
}

void Client::recconectToServer()
{
	socket.reset();
	if (!clientThread)
	{
		clientThread = std::make_unique<std::thread>(&Client::establishConnection, this, socket.getServerEndpoint());
	}
}

void Client::requestData(Json& recievedData, Result& result)
{
	if (!clientThread)
	{
		clientThread = std::make_unique<std::thread>(&Client::requestDataFromSerever, this, std::ref(recievedData), std::ref(result));
	}
}

void Client::recieveData(Json& recievedData, Result& result)
{
	if (!clientThread)
	{
		clientThread = std::make_unique<std::thread>(&Client::recieveDataT, this, std::ref(recievedData), std::ref(result));
	}
}

void Client::join()
{
	if (clientThread)
	{
		clientThread->join();
		clientThread.reset();
	}
}

void Client::establishConnection(const IPEndpoint& serverEndpoint)
{
	LOG << "trying to connect to "  + serverEndpoint.toString() + '\n';
	while (socket.connect(serverEndpoint) != Result::Success);
	LOG << "connected to " + serverEndpoint.toString() + '\n';
}

void Client::requestDataFromSerever(Json& dataRecieved, Result& result)
{
	LOG << "requesting data from " + socket.toString() + '\n';
	dataRecieved.clear();
	char data = 0;
	int bytesSend = 0;
	result = socket.send(reinterpret_cast<const void*>(&data), sizeof(data), bytesSend);
	if (result == Result::Success)
	{
		result = socket.recieveJson(dataRecieved);
		LOG << "recieved data from " + socket.toString() + " data recieved: " + dataRecieved.dump() + '\n';
	}
	else
	{
		LOG << "could not recieved data from " + socket.toString() + '\n';
	}
}

void Client::recieveDataT(Json& recievedData, Result& result)
{
	result = socket.recieveJson(recievedData);
}
