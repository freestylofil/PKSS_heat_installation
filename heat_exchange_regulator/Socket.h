#pragma once
#include "SocketHandle.h"
#include "Result.h"
#include "IPVersion.h"
#include "SocketOption.h"
#include "IPEndpoint.h"
#include "Json.h"

using Json = nlohmann::json;

class Socket
{
public:
	Socket(IPVersion ipversion = IPVersion::IPv4, SocketHandle handle = INVALID_SOCKET);
	Result create();
	Result createBroadcast();
	virtual Result bind(IPEndpoint endpoint);
	Result close();
	Result reset();
	Result send(const void* data, int numberOfBytes, int& bytesSent);
	Result recieve(void* destination, int numberOfBytes, int& bytesRecieved);
	Result sendAll(const void* data, int numberOfBytes);
	Result recieveAll(void* destination, int numberOfBytes);
	Result sendJson(const Json& jsonData);
	Result recieveJson(Json& jsonDestination);
	Result setSocketOption(SocketOption option, BOOL value);
	Result getIPEndpoint(IPEndpoint& ipEndpoint) const;
	SocketHandle getHandle() const;
	IPVersion getIPVersion() const;
	std::string toString() const;
	virtual ~Socket() = default;
protected:
	IPVersion ipversion = IPVersion::IPv4;
	SocketHandle handle = INVALID_SOCKET;
};

std::ostream& operator<<(std::ostream& stream, const Socket& socket);