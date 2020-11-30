#pragma once
#include "Socket.h"
#include <json.h>

using Json = nlohmann::json;

class ServerSocket : public Socket
{
public:
	ServerSocket(IPVersion ipversion = IPVersion::IPv4, SocketHandle handle = INVALID_SOCKET);
	Result listen(int backlog = 5) const;
	Result accept(ServerSocket& outSocket) const;
};
