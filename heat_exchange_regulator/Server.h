#pragma once
#include "ServerSocket.h"
#include "TransmissionType.h"
#include <atomic>
#include <list>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>

class Server
{
public:
	Server(TransmissionType transmissionType, unsigned long timeout);
	void start(IPEndpoint serverEndpoint, Json defaultData, unsigned short portOut = 0);
	void bindData(const Json& data);
	~Server();
private:
	Json getData();
	void listen(int backlog = 5);
	void startWaitingForRequests(ServerSocket&& socket);
private:
	std::atomic<unsigned short> portOut;
	unsigned long timeout;
	std::atomic<TransmissionType> transmissionType;
	ServerSocket socket;
	std::unique_ptr<std::thread> serverThread;
	Json data;
	std::atomic<bool> alive = true;
	std::mutex mutex;
};

