#include "Socket.h"
#include <assert.h>
#include <sstream>


Socket::Socket(IPVersion ipversion, SocketHandle handle)
    : ipversion(ipversion), handle(handle)
{
    assert(ipversion == IPVersion::IPv4);
}

Result Socket::create()
{
    assert(ipversion == IPVersion::IPv4);
    if (handle != INVALID_SOCKET)
    {
        return Result::GenericError;
    }

    handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (handle == INVALID_SOCKET)
    {
        int error = WSAGetLastError();
        return Result::GenericError;
    }

    if (setSocketOption(SocketOption::TCP_NoDelay, TRUE) != Result::Success)
    {
        return Result::GenericError;
    }

    return Result::Success;
}

Result Socket::createBroadcast()
{
    assert(ipversion == IPVersion::IPv4);
    if (handle != INVALID_SOCKET)
    {
        return Result::GenericError;
    }

    handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (handle == INVALID_SOCKET)
    {
        int error = WSAGetLastError();
        return Result::GenericError;
    }

    if (setSocketOption(SocketOption::SO_Broadcast, TRUE) != Result::Success)
    {
        return Result::GenericError;
    }

    return Result::Success;
}

Result Socket::bind(IPEndpoint endpoint)
{
    sockaddr_in addr = endpoint.getSockaddrIPv4();
    int result = ::bind(handle, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in));
    if (result != 0)
    {
        int error = WSAGetLastError();
        return Result::GenericError;
    }
    return Result::Success;
}

Result Socket::close()
{
    if (handle == INVALID_SOCKET)
    {
        return Result::GenericError;
    }

    int result = closesocket(handle);
    if (result != 0)
    {
        int error = WSAGetLastError();
        return Result::GenericError;
    }

    handle = INVALID_SOCKET;
    return Result::Success;
}

Result Socket::reset()
{
    handle = INVALID_SOCKET;
    return create();
}

Result Socket::send(const void* data, int numberOfBytes, int& bytesSent)
{
    bytesSent = ::send(handle, reinterpret_cast<const char*>(data), numberOfBytes, NULL);
    if (bytesSent == SOCKET_ERROR)
    {
        return errorCodeToResult(WSAGetLastError());
    }
    return Result::Success;
}

Result Socket::recieve(void* destination, int numberOfBytes, int& bytesRecieved)
{
    bytesRecieved = recv(handle, reinterpret_cast<char*>(destination), numberOfBytes, NULL);
    if (bytesRecieved == 0)
    {
        return Result::GenericError;
    }
    if (bytesRecieved == SOCKET_ERROR)
    {
        int error = WSAGetLastError();
        return Result::GenericError;
    }
    return Result::Success;
}

Result Socket::sendAll(const void* data, int numberOfBytes)
{
    int totalBytesSent = 0;
    while (totalBytesSent < numberOfBytes)
    {
        int bytesRemaining = numberOfBytes - totalBytesSent;
        int bytesSent = 0;
        const char* bufferOffset = reinterpret_cast<const char*>(data) + totalBytesSent;
        Result result = send(bufferOffset, numberOfBytes, bytesSent);
        if (result != Result::Success)
        {
            return Result::GenericError;
        }
        totalBytesSent += bytesSent;
    }
    return Result::Success;
}

Result Socket::sendJson(const Json& jsonData)
{
    std::string jsonDataStr = jsonData.dump();
    jsonDataStr.shrink_to_fit();
    return sendAll(jsonData.dump().c_str(), jsonDataStr.size());
}

Result Socket::recieveJson(Json& jsonDestination)
{
    std::string buffer;
    int bytesRecieved;
    buffer.resize(256);
    if (recieve(&buffer[0], 256, bytesRecieved) == Result::Success)
    {
        std::istringstream oss(buffer);
        oss >> jsonDestination;
        return Result::Success;
    }
    return Result::GenericError;
}

Result Socket::getIPEndpoint(IPEndpoint& ipEndpoint) const
{
    sockaddr addr;
    int addrLen = sizeof(sockaddr_in);
    int result = getsockname(handle, &addr, &addrLen);
    if (result != 0)
    {
        int error = WSAGetLastError();
        return Result::GenericError;
    }
    ipEndpoint = IPEndpoint(&addr);
    return Result::Success;
}

Result Socket::recieveAll(void* destination, int numberOfBytes)
{
    int totalBytesRecieved = 0;
    while (totalBytesRecieved < numberOfBytes)
    {
        int bytesRemaining = numberOfBytes - totalBytesRecieved;
        int bytesRecieved = 0;
        char* bufferOffset = reinterpret_cast<char*>(destination) + totalBytesRecieved;
        Result result = recieve(bufferOffset, numberOfBytes, bytesRecieved);
        if (result != Result::Success)
        {
            return Result::GenericError;
        }
        totalBytesRecieved += bytesRecieved;
    }
    return Result::Success;
}

SocketHandle Socket::getHandle() const
{
    return handle;
}

IPVersion Socket::getIPVersion() const
{
    return ipversion;
}
std::string Socket::toString() const
{
    IPEndpoint endpoint;
    if (getIPEndpoint(endpoint) == Result::Success)
    {
        return std::to_string(handle) + " -> " + endpoint.toString();
    }
    return "unkown endpoint";
}
Result Socket::setSocketOption(SocketOption option, BOOL value)
{
    int result = 0;
    switch (option)
    {
    case SocketOption::TCP_NoDelay:
        result = setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&value), sizeof(value));
        break;
    case SocketOption::SO_Broadcast:
        result = setsockopt(handle, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<const char*>(&value), sizeof(value));
        break;
    default:
        return Result::GenericError;
    }

    if (result != 0)
    {
        int error = WSAGetLastError();
        return Result::GenericError;
    }
    return Result::Success;
}

std::ostream& operator<<(std::ostream& stream, const Socket& socket)
{
    return stream << socket.toString();
}
