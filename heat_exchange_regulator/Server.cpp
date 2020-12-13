#include "Server.h"
#include "Logger.h"

Server::Server(TransmissionType transmissionType, unsigned long timeout)
	: transmissionType(transmissionType), timeout(timeout)
{
}

void Server::start(IPEndpoint serverEndpoint, Json defaultData, unsigned short portOut)
{
	bindData(defaultData);
	this->portOut.store(portOut);
	socket.create(transmissionType, timeout);
	socket.bind(serverEndpoint);
	if (transmissionType == TransmissionType::unicast)
	{
		serverThread = std::make_unique<std::thread>(&Server::listen, this, 5);
	}
	else
	{
		serverThread = std::make_unique<std::thread>(&Server::startWaitingForRequests, this, socket);
	}
	LOG << "server on " + serverEndpoint.toString() + " successfuly started\n";
}

void Server::bindData(const Json& data)
{
	mutex.lock();
	this->data = data;
	mutex.unlock();
	LOG << "data: " + data.dump() + " bounded to server\n";
}

Server::~Server()
{
	alive.store(false);
	if (serverThread)
	{
		serverThread->join();
		serverThread.reset();
	}
}

Json Server::getData()
{
	mutex.lock();
	Json dataOut = data;
	mutex.unlock();
	return dataOut;
}

void Server::listen(int backlog)
{
	while (alive.load())
	{
		if (socket.listen(backlog) == Result::success)
		{
			ServerSocket respondingSocket;
			if (socket.accept(respondingSocket) == Result::success)
			{
				LOG << "Added client on " + respondingSocket.toString() << '\n';
				std::thread respondingThread(&Server::startWaitingForRequests, this, std::move(respondingSocket));
				respondingThread.detach();
			}
		}
	}
}

void Server::startWaitingForRequests(ServerSocket&& socket)
{
	char recievedData[256];
	int bytesRecieved;
	while (alive.load())
	{
		if (socket.recieve(recievedData, 256, bytesRecieved) == Result::success)
		{
			if (transmissionType.load() == TransmissionType::unicast)
			{
				socket.sendJson(getData());
			}
			else
			{
				socket.sendJsonBroadcast(getData(), portOut.load());
			}
			LOG << "Responded on " + socket.toString() + " request\n";
		}
	}
}