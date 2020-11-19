#include "ServerSocket.h"
#include <assert.h>
#include <sstream>


ServerSocket::ServerSocket(IPVersion ipversion, SocketHandle handle)
    : Socket(ipversion, handle)
{
    assert(ipversion == IPVersion::IPv4);
}

Result ServerSocket::listen(int backlog) const
{
    int result = ::listen(handle, backlog);
    if (result != 0)
    {
        int error = WSAGetLastError();
        return Result::GenericError;
    }

    return Result::Success;
}

Result ServerSocket::accept(ServerSocket& outSocket) const
{
    sockaddr_in addr = {};
    int len = sizeof(sockaddr_in);
    SocketHandle acceptedConnetionHandle = ::accept(handle, reinterpret_cast<sockaddr*>(&addr), &len);
    if (acceptedConnetionHandle == INVALID_SOCKET)
    {
        int error = WSAGetLastError();
        return Result::GenericError;
    }
    IPEndpoint newConnectionEndpoint(reinterpret_cast<sockaddr*>(&addr));
    outSocket = ServerSocket(IPVersion::IPv4, acceptedConnetionHandle);
    return Result::Success;
}
