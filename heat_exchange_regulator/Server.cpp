#include "Server.h"
#include "Logger.h"

void Server::start(IPEndpoint serverEndpoint)
{
	lisningSocket.create();
	lisningSocket.bind(serverEndpoint);
	lisningSocketThread = std::make_unique<std::thread>(&Server::listen, this, 5);
	LOG << "server on " + serverEndpoint.toString() + " successfuly started\n";
}

void Server::bindData(const Json& data)
{
	this->data = data;
	LOG << "data: " + data.dump() + " bounded to server\n";
}

void Server::listen(int backlog)
{
	while (lisningSocket.listen(backlog) == Result::Success)
	{
		ServerSocket respondingSocket;
		if (lisningSocket.accept(respondingSocket) == Result::Success)
		{
			std::thread respondingThread(&Server::startWaitingForRequests, this, std::move(respondingSocket));
			respondingThread.detach();
			LOG << "Added client on " + respondingSocket.toString() << '\n';
		}
	}
}

void Server::startWaitingForRequests(ServerSocket socket)
{
	char recievedData[256];
	int bytesRecieved;
	while (socket.recieve(recievedData, 256, bytesRecieved) == Result::Success)
	{
		socket.sendJson(data);
		LOG << "Responded on " + socket.toString() + " request\n";
	}
}