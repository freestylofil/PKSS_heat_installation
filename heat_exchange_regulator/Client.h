#pragma once
#include "ClientSocket.h"
#include "TransmissionType.h"
#include <memory>
#include <thread>
#include <atomic>

class Client
{
public:
	Client(IPEndpoint serverEndpoint, TransmissionType transmissionType, unsigned long timeout);
	void connectToServer();
	void recconectToServer(Result& result);
	void bind(IPEndpoint endpoint);
	void requestData(Json& dataRecieved, Result& result);
	void recieveData(Json& dataRecieved, Result& result);
	void requestTime(unsigned long long& time, Result& result);
	void logDataToServer(Json& data, long long time, Result& result);
	void join();
	TransmissionType getTransmissionType() const;
	~Client() noexcept;
private: 
	void establishConnection(const IPEndpoint& serverEndpoint);
	void reestablishConnection(const IPEndpoint& serverEndpoint, Result& result);
	void requestDataFromSerever(Json& dataRecieved, Result& result);
	void requestTimeFromSerever(unsigned long long& time, Result& result);
	void recieveDataT(Json& recievedData, Result& result);
	void logData(Json data, long long time, Result& result);
private:
	IPEndpoint serverEndpoint;
	TransmissionType transmissionType;
	unsigned long timeout;
	ClientSocket socket;
	std::unique_ptr<std::thread> clientThread;
	std::atomic<bool> alive = true;
};


