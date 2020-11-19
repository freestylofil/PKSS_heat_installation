#include "ClientSocket.h"
#include <assert.h>
#include <sstream>


ClientSocket::ClientSocket(IPVersion ipversion, SocketHandle handle)
    : Socket(ipversion, handle)
{
    assert(ipversion == IPVersion::IPv4);
}

Result ClientSocket::bind(IPEndpoint endpoint)
{
    sockaddr_in addr = endpoint.getSockaddrIPv4();
    return Socket::bind(endpoint);
}
Result ClientSocket::connect(IPEndpoint endpoint)
{
    serverEndpoint = endpoint;
    sockaddr_in addr = endpoint.getSockaddrIPv4();
    int result = ::connect(handle, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in));
    if (result != 0)
    {
        int error = WSAGetLastError();
        return Result::GenericError;
    }
    return Result::Success;
}

IPEndpoint ClientSocket::getServerEndpoint() const
{
    return serverEndpoint;
}
