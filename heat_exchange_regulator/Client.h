#pragma once
#include "ClientSocket.h"
#include <memory>
#include <thread>

class Client
{
public:
	Client() = default;
	void connectToServer(IPEndpoint serverEndpoint);
	void bindToServer(IPEndpoint serverEndpoint);
	void recconectToServer();
	void requestData(Json& recievedData, Result& result);
	void recieveData(Json& recievedData, Result& result);
	void join();
private:
	void establishConnection(const IPEndpoint& serverEndpoint);
	void requestDataFromSerever(Json& dataRecieved, Result& result);
	void recieveDataT(Json& recievedData, Result& result);
private:
	ClientSocket socket;
	std::unique_ptr<std::thread> clientThread;
};


