#pragma once
#include "ServerSocket.h"
#include <atomic>
#include <list>
#include <memory>
#include <thread>

class Server
{
public:
	Server() = default;
	void start(IPEndpoint serverEndpoint);
	void bindData(const Json& data);
private:
	void listen(int backlog = 5);
	void startWaitingForRequests(ServerSocket socket);
private:
	ServerSocket lisningSocket;
	std::unique_ptr<std::thread> lisningSocketThread;
	Json data;
};

