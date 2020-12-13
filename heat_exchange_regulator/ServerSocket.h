#pragma once
#include "Socket.h"

using Json = nlohmann::json;

class ServerSocket : public Socket
{
public:
	ServerSocket(IPVersion ipversion = IPVersion::IPv4, SocketHandle handle = INVALID_SOCKET);
	Result listen(int backlog = 5);
	Result accept(ServerSocket& outSocket) const;
};
