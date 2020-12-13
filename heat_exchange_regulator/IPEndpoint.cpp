#include "IPEndpoint.h"
#include <WS2tcpip.h>
#include <assert.h>

IPEndpoint::IPEndpoint(const char* ip, unsigned short port)
    : port(port)
{
    //IPv4
    in_addr addr;
    int result = inet_pton(AF_INET, ip, &addr);

    if (result == 1)
    {
        if (addr.S_un.S_addr != INADDR_NONE)
        {
            ipString = ip;
            hostname = ip;
            ipBytes.resize(sizeof(ULONG));
            memcpy(ipBytes.data(), &addr.S_un.S_addr, sizeof(ULONG));
            ipversion = IPVersion::IPv4;
            return;
        }
    }

    addrinfo hints = {};
    hints.ai_family = AF_INET;
    addrinfo* hostinfo = nullptr;
    result = getaddrinfo(ip, NULL, &hints, &hostinfo);
    if (result == 0)
    {
        sockaddr_in* hostAddr = reinterpret_cast<sockaddr_in*>(hostinfo->ai_addr);
        ipString.resize(16);
        inet_ntop(AF_INET, &hostAddr->sin_addr, &ipString[0], 16);

        hostname = ip;
        ULONG ipLong = hostAddr->sin_addr.S_un.S_addr;
        ipBytes.resize(sizeof(LONG));
        memcpy(ipBytes.data(), &ipLong, sizeof(ULONG));
        ipversion = IPVersion::IPv4;

        freeaddrinfo(hostinfo);
        return;
    }

    //IPv6
    in6_addr addr6;
    result = inet_pton(AF_INET6, ip, &addr6);

    if (result == 1)
    {
         ipString = ip;
         hostname = ip;
         ipBytes.resize(16);
         memcpy(ipBytes.data(), &addr6.u, 16);
         ipversion = IPVersion::IPv6;
         return;
    }

    addrinfo hintsv6 = {};
    hints.ai_family = AF_INET6;
    addrinfo* hostinfov6 = nullptr;
    result = getaddrinfo(ip, NULL, &hintsv6, &hostinfov6);
    if (result == 0)
    {
        sockaddr_in6* hostAddr = reinterpret_cast<sockaddr_in6*>(hostinfov6->ai_addr);
        ipString.resize(46);
        inet_ntop(AF_INET6, &hostAddr->sin6_addr, &ipString[0], 46);

        hostname = ip;
        ipBytes.resize(16);
        memcpy(ipBytes.data(),  &hostAddr->sin6_addr, 16);
        ipversion = IPVersion::IPv6;

        freeaddrinfo(hostinfo);
        return;
    }
}

IPEndpoint::IPEndpoint(const sockaddr* addr)
{
    assert(addr->sa_family == AF_INET || addr->sa_family == AF_INET6);
    if (addr->sa_family == AF_INET) //IPv4
    {
        const sockaddr_in* addrv4 = reinterpret_cast<const sockaddr_in*>(addr);
        ipversion = IPVersion::IPv4;
        port = ntohs(addrv4->sin_port);
        ipBytes.resize(sizeof(ULONG));
        memcpy(ipBytes.data(), &addrv4->sin_addr.S_un, sizeof(ULONG));
        ipString.resize(16);
        inet_ntop(AF_INET, &addrv4->sin_addr, &ipString[0], 16);
        hostname = ipString;
    }
    else //IPv6
    {
        const sockaddr_in6* addrv6 = reinterpret_cast<const sockaddr_in6*>(addr);
        ipversion = IPVersion::IPv6;
        port = ntohs(addrv6->sin6_port);
        ipBytes.resize(16);
        memcpy(ipBytes.data(), &addrv6->sin6_addr, 16);
        ipString.resize(46);
        inet_ntop(AF_INET6, &addrv6->sin6_addr, &ipString[0], 46);
        hostname = ipString;
    }
}

IPVersion IPEndpoint::getIPVersion() const
{
    return ipversion;
}

const std::string& IPEndpoint::getHostName() const
{
    return hostname;
}

const std::string& IPEndpoint::getIpString() const
{
    return ipString;
}

const std::vector<uint8_t>& IPEndpoint::getIpBytes() const
{
    return ipBytes;
}

unsigned short IPEndpoint::getPort() const
{
    return port;
}

sockaddr_in IPEndpoint::getSockaddrIPv4() const
{
    assert(ipversion == IPVersion::IPv4);
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    memcpy(&addr.sin_addr, ipBytes.data(), sizeof(ULONG));
    addr.sin_port = htons(port);
    return addr;
}

sockaddr_in6 IPEndpoint::getSockaddrIPv6() const
{
    assert(ipversion == IPVersion::IPv6);
    sockaddr_in6 addr = {};
    addr.sin6_family = AF_INET6;
    memcpy(&addr.sin6_addr, ipBytes.data(), 16);
    addr.sin6_port = htons(port);
    return addr;
}

std::string IPEndpoint::toString() const
{
    return '(' + getIpString() + " : " + std::to_string(getPort()) + ')';
}

std::ostream& operator<<(std::ostream& ostream, const IPEndpoint& endpoint)
{
    return ostream << endpoint.toString();
}
