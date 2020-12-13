#pragma once
#include "SocketHandle.h"
#include "Result.h"
#include "IPVersion.h"
#include "SocketOption.h"
#include "IPEndpoint.h"
#include "Json.h"
#include "TransmissionType.h"
#include "IOMode.h"

using Json = nlohmann::json;

class Socket
{
public:
	Socket(IPVersion ipversion = IPVersion::IPv4, SocketHandle handle = INVALID_SOCKET);
	Socket(const Socket&) = default;
	Socket(Socket&& source);
	Socket& operator=(const Socket&) = default;
	Socket& operator=(Socket&& rhs);
	virtual ~Socket();
	//general methods
	void create(TransmissionType transmissionType, DWORD timeout);
	Result bind(IPEndpoint endpoint);
	Result setSocketOption(SocketOption option, BOOL value);
	Result setSocketOption(SocketOption option, DWORD value);
	Result setIOMode(IOMode mode, unsigned long arg);
	Result close();
	//sending methods
	Result send(const void* data, int numberOfBytes, int& bytesSent);
	Result sendAll(const void* data, int numberOfBytes);
	Result sendJson(const Json& jsonData);
	Result sendBroadcast(const void* data, int numberOfBytes, int& bytesSent, unsigned short port);
	Result sendAllBroadcast(const void* data, int numberOfBytes, unsigned short port);
	Result sendJsonBroadcast(const Json& jsonData, unsigned short port);
	Result sendTo(const void* data, int numberOfBytes, int& bytesSent, IPEndpoint endpoint);
	Result sendAllTo(const void* data, int numberOfBytes, IPEndpoint endpoint);
	Result sendJsonTo(const Json& jsonData, IPEndpoint endpoint);
	//recieving methods
	Result recieve(void* destination, int numberOfBytes, int& bytesRecieved);
	Result recieveAll(void* destination, int numberOfBytes);
	Result recieveJson(Json& jsonDestination);
	Result recieveTime(unsigned long long& time);
	//getters
	Result getIPEndpoint(IPEndpoint& ipEndpoint) const;
	SocketHandle getHandle() const;
	IPVersion getIPVersion() const;
	//toString
	std::string toString() const;
protected:
	IPVersion ipversion = IPVersion::IPv4;
	SocketHandle handle = INVALID_SOCKET;
};

std::ostream& operator<<(std::ostream& stream, const Socket& socket);