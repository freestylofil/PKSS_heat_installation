#pragma once
#include "Socket.h"

using Json = nlohmann::json;

class ClientSocket : public Socket
{
public:
	ClientSocket(IPVersion ipversion = IPVersion::IPv4, SocketHandle handle = INVALID_SOCKET);
	Result connect(IPEndpoint endpoint);
	IPEndpoint getServerEndpoint() const;
private:
	IPEndpoint serverEndpoint = {};
};