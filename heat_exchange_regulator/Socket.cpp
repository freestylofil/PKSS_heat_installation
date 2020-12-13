#include "Socket.h"
#include "NetworkException.h"
#include <assert.h>
#include <sstream>


Socket::Socket(IPVersion ipversion, SocketHandle handle)
    : ipversion(ipversion), handle(handle)
{
    assert(ipversion == IPVersion::IPv4 || ipversion == IPVersion::IPv6);
}

Socket::Socket(Socket&& source)
    : ipversion(source.ipversion), handle(source.handle)
{
    source.ipversion = IPVersion::Unknown;
    source.handle = INVALID_SOCKET;
}

Socket& Socket::operator=(Socket&& rhs)
{
    if (this == &rhs)
    {
        return *this;
    }
    if (handle != INVALID_SOCKET)
    {
        close();
    }
    ipversion = rhs.ipversion;
    handle = rhs.handle;
    rhs.ipversion = IPVersion::Unknown;
    rhs.handle = INVALID_SOCKET;
    return *this;
}

void Socket::create(TransmissionType transmissionType, DWORD timeout)
{
    assert(ipversion == IPVersion::IPv4 || ipversion == IPVersion::IPv6);
    if (handle != INVALID_SOCKET)
    {
        throw std::runtime_error("Invalid socket");
    }

    if (transmissionType == TransmissionType::unicast)
    {
        handle = socket(ipversion == IPVersion::IPv4 ? AF_INET : AF_INET6, SOCK_STREAM, IPPROTO_TCP);
        if (handle == INVALID_SOCKET)
        {
            int errorCode = WSAGetLastError();
            throw NETWORK_EXCEPTION(errorCode);
        }

        if (setSocketOption(SocketOption::TCP_NoDelay, TRUE) != Result::success)
        {
            int errorCode = WSAGetLastError();
            throw NETWORK_EXCEPTION(errorCode);
        }
    }
    else
    {
        handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (handle == INVALID_SOCKET)
        {
            int errorCode = WSAGetLastError();
            throw NETWORK_EXCEPTION(errorCode);
        }

        if (setSocketOption(SocketOption::SO_Broadcast, TRUE) != Result::success)
        {
            int errorCode = WSAGetLastError();
            throw NETWORK_EXCEPTION(errorCode);
        }
    }
    if (setSocketOption(SocketOption::SO_RecieveTimeout, timeout) != Result::success)
    {
        int errorCode = WSAGetLastError();
        throw NETWORK_EXCEPTION(errorCode);
    }
    if (setIOMode(IOMode::fionbio, 1ul) != Result::success)
    {
        int errorCode = WSAGetLastError();
        throw NETWORK_EXCEPTION(errorCode);
    }
}

Result Socket::bind(IPEndpoint endpoint)
{
    assert(ipversion == endpoint.getIPVersion());
    if (ipversion == IPVersion::IPv4) //IPv4
    {
        sockaddr_in addr = endpoint.getSockaddrIPv4();
        int result = ::bind(handle, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in));
        if (result != 0)
        {
            int errorCode = WSAGetLastError();
            throw NETWORK_EXCEPTION(errorCode);
        }
    }
    else //IPv6
    {
        sockaddr_in6 addr = endpoint.getSockaddrIPv6();
        int result = ::bind(handle, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in6));
        if (result != 0)
        {
            int errorCode = WSAGetLastError();
            throw NETWORK_EXCEPTION(errorCode);
        }
    }
    return Result::success;
}

Result Socket::close()
{
    if (handle == INVALID_SOCKET)
    {
        return Result::genericError;
    }

    int result = closesocket(handle);
    if (result != 0)
    {
        int errorCode = WSAGetLastError();
        throw NETWORK_EXCEPTION(errorCode);
    }

    handle = INVALID_SOCKET;
    return Result::success;
}

Result Socket::send(const void* data, int numberOfBytes, int& bytesSent)
{
    bytesSent = ::send(handle, reinterpret_cast<const char*>(data), numberOfBytes, NULL);
    if (bytesSent == SOCKET_ERROR)
    {
        return errorCodeToResult(WSAGetLastError());
    }
    return Result::success;
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
        if (result != Result::success)
        {
            return Result::genericError;
        }
        totalBytesSent += bytesSent;
    }
    return Result::success;
}

Result Socket::sendJson(const Json& jsonData)
{
    std::string jsonDataStr = jsonData.dump();
    jsonDataStr.shrink_to_fit();
    return sendAll(jsonDataStr.c_str(), (int)jsonDataStr.size());
}

Result Socket::sendBroadcast(const void* data, int numberOfBytes, int& bytesSent, unsigned short port)
{
    return sendTo(data, numberOfBytes, bytesSent, { "255.255.255.255", port });
}

Result Socket::sendAllBroadcast(const void* data, int numberOfBytes, unsigned short port)
{
    int totalBytesSent = 0;
    while (totalBytesSent < numberOfBytes)
    {
        int bytesRemaining = numberOfBytes - totalBytesSent;
        int bytesSent = 0;
        const char* bufferOffset = reinterpret_cast<const char*>(data) + totalBytesSent;
        Result result = sendBroadcast(bufferOffset, numberOfBytes, bytesSent, port);
        if (result != Result::success)
        {
            return Result::genericError;
        }
        totalBytesSent += bytesSent;
    }
    return Result::success;
}

Result Socket::sendJsonBroadcast(const Json& jsonData, unsigned short port)
{
    std::string jsonDataStr = jsonData.dump();
    jsonDataStr.shrink_to_fit();
    return sendAllBroadcast(jsonDataStr.c_str(), (int)jsonDataStr.size(), port);
}

Result Socket::sendTo(const void* data, int numberOfBytes, int& bytesSent, IPEndpoint endpoint)
{
    sockaddr_in addr = endpoint.getSockaddrIPv4();
    bytesSent = ::sendto(handle, reinterpret_cast<const char*>(data), numberOfBytes, NULL, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in));
    if (bytesSent == SOCKET_ERROR)
    {
        return errorCodeToResult(WSAGetLastError());
    }
    return Result::success;
}

Result Socket::sendAllTo(const void* data, int numberOfBytes, IPEndpoint endpoint)
{
    int totalBytesSent = 0;
    while (totalBytesSent < numberOfBytes)
    {
        int bytesRemaining = numberOfBytes - totalBytesSent;
        int bytesSent = 0;
        const char* bufferOffset = reinterpret_cast<const char*>(data) + totalBytesSent;
        Result result = sendTo(bufferOffset, numberOfBytes, bytesSent, endpoint);
        if (result != Result::success)
        {
            return Result::genericError;
        }
        totalBytesSent += bytesSent;
    }
    return Result::success;
}

Result Socket::sendJsonTo(const Json& jsonData, IPEndpoint endpoint)
{
    std::string jsonDataStr = jsonData.dump();
    jsonDataStr.shrink_to_fit();
    return sendAllTo(jsonDataStr.c_str(), (int)jsonDataStr.size(), endpoint);
}


Result Socket::recieve(void* destination, int numberOfBytes, int& bytesRecieved)
{
    bytesRecieved = recv(handle, reinterpret_cast<char*>(destination), numberOfBytes, NULL);
    if (bytesRecieved == 0)
    {
        return Result::genericError;
    }
    if (bytesRecieved == SOCKET_ERROR)
    {
        int errorCode = WSAGetLastError();
        return errorCodeToResult(errorCode);
    }
    return Result::success;
}

Result Socket::recieveJson(Json& jsonDestination)
{
    std::string buffer;
    int bytesRecieved;
    buffer.resize(256);
    Result result = recieve(&buffer[0], 256, bytesRecieved);
    if (result == Result::success)
    {
        std::istringstream oss(buffer);
        oss >> jsonDestination;
        return Result::success;
    }
    return result;
}


Result Socket::recieveTime(unsigned long long& time)
{
    std::string buffer;
    int bytesRecieved;
    buffer.resize(256);
    Result result = recieve(&buffer[0], 256, bytesRecieved);
    if (result == Result::success)
    {
        std::memcpy(&time, &buffer[16], sizeof(unsigned long long));
        time = _byteswap_uint64(time) / 10000000000;
        return Result::success;
    }
    return result;
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
        if (result != Result::success)
        {
            return result;
        }
        totalBytesRecieved += bytesRecieved;
    }
    return Result::success;
}

Result Socket::getIPEndpoint(IPEndpoint& ipEndpoint) const
{
    sockaddr addr;
    int addrLen = sizeof(sockaddr_in);
    int result = getsockname(handle, &addr, &addrLen);
    if (result != 0)
    {
        int error = WSAGetLastError();
        return Result::genericError;
    }
    ipEndpoint = IPEndpoint(&addr);
    return Result::success;
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
    if (getIPEndpoint(endpoint) == Result::success)
    {
        return std::to_string(handle) + " -> " + endpoint.toString();
    }
    return "unkown endpoint";
}

Socket::~Socket()
{
    if (handle != INVALID_SOCKET)
    {
        close();
    }
}

Result Socket::setSocketOption(SocketOption option, BOOL value)
{
    int result = 0;
    switch (option)
    {
    case SocketOption::TCP_NoDelay:
        result = setsockopt(handle, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&value), sizeof(value));
        break;
    case SocketOption::IPV6_Only:
        result = setsockopt(handle, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast<const char*>(&value), sizeof(value));
        break;
    case SocketOption::SO_Broadcast:
        result = setsockopt(handle, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<const char*>(&value), sizeof(value));
        break;
    default:
        return Result::genericError;
    }

    if (result != 0)
    {
        return Result::genericError;
    }
    return Result::success;
}

Result Socket::setSocketOption(SocketOption option, DWORD value)
{
    int result = 0;
    switch (option)
    {
    case SocketOption::SO_RecieveTimeout:
        result = setsockopt(handle, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char*>(&value), sizeof(value));
        break;
    default:
        return Result::genericError;
    }

    if (result != 0)
    {
        return Result::genericError;
    }
    return Result::success;
}

Result Socket::setIOMode(IOMode mode, unsigned long arg)
{
    int result = 0;
    switch (mode)
    {
    case IOMode::fionbio:
        result = ioctlsocket(handle, FIONBIO, &arg);
        break;
    default:
        return Result::genericError;
    }

    if (result != 0)
    {
        return Result::genericError;
    }
    return Result::success;
}

std::ostream& operator<<(std::ostream& stream, const Socket& socket)
{
    return stream << socket.toString();
}
