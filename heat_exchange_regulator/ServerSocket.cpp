#include "ServerSocket.h"
#include <assert.h>
#include <sstream>


ServerSocket::ServerSocket(IPVersion ipversion, SocketHandle handle)
    : Socket(ipversion, handle)
{
}

Result ServerSocket::listen(int backlog)
{
    assert(ipversion == IPVersion::IPv4 || ipversion == IPVersion::IPv6);
    if (ipversion == IPVersion::IPv6)
    {
        if (setSocketOption(SocketOption::IPV6_Only, FALSE) != Result::success)
        {
            return Result::genericError;
        }
    }

    int result = ::listen(handle, backlog);
    if (result != 0)
    {
        return Result::genericError;
    }

    return Result::success;
}

Result ServerSocket::accept(ServerSocket& outSocket) const
{
    assert(ipversion == IPVersion::IPv4 || ipversion == IPVersion::IPv6);
    if (ipversion == IPVersion::IPv4) //IPv4
    {
        sockaddr_in addr = {};
        int len = sizeof(sockaddr_in);
        SocketHandle acceptedConnetionHandle = ::accept(handle, reinterpret_cast<sockaddr*>(&addr), &len);
        if (acceptedConnetionHandle == INVALID_SOCKET)
        {
            int errorCode = WSAGetLastError();
            return errorCodeToResult(errorCode);
        }
        IPEndpoint newConnectionEndpoint(reinterpret_cast<sockaddr*>(&addr));
        outSocket = ServerSocket(IPVersion::IPv4, acceptedConnetionHandle);

        return Result::success;
    }
    else //IPv6
    {
        sockaddr_in6 addr = {};
        int len = sizeof(sockaddr_in6);
        SocketHandle acceptedConnetionHandle = ::accept(handle, reinterpret_cast<sockaddr*>(&addr), &len);
        if (acceptedConnetionHandle == INVALID_SOCKET)
        {
            return Result::genericError;
        }
        IPEndpoint newConnectionEndpoint(reinterpret_cast<sockaddr*>(&addr));
        outSocket = ServerSocket(IPVersion::IPv6, acceptedConnetionHandle);
        return Result::success;
    }
}
