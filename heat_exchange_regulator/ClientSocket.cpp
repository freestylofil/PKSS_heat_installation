#include "ClientSocket.h"
#include <assert.h>
#include <sstream>


ClientSocket::ClientSocket(IPVersion ipversion, SocketHandle handle)
    : Socket(ipversion, handle)
{
}

Result ClientSocket::connect(IPEndpoint endpoint)
{
    assert(ipversion == IPVersion::IPv4 || ipversion == IPVersion::IPv6);
    serverEndpoint = endpoint;
    int result = 0;
    if (ipversion == IPVersion::IPv4) //IPv4
    {
        sockaddr_in addr = endpoint.getSockaddrIPv4();
        result = ::connect(handle, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in));
    }
    else //IPv6
    {
        sockaddr_in6 addr = endpoint.getSockaddrIPv6();
        result = ::connect(handle, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in6));
    }
    if (result != 0)
    {
        int errorCode = WSAGetLastError();
        if (errorCode == WSAEISCONN)
        {
            return Result::success;
        }
        return errorCodeToResult(errorCode);
    }
    return Result::success;
}

IPEndpoint ClientSocket::getServerEndpoint() const
{
    return serverEndpoint;
}
