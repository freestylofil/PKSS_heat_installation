#pragma once
#include "IPVersion.h"
#include <string>
#include <vector>
#include <WS2tcpip.h>
#include <iostream>


class IPEndpoint
{
public:
	IPEndpoint() = default;
	IPEndpoint(const char* ip, unsigned short port);
	IPEndpoint(const sockaddr* addr);
	IPVersion getIPVersion() const;
	const std::string& getHostName() const;
	const std::string& getIpString() const;
	const std::vector<uint8_t>& getIpBytes() const;
	unsigned short getPort() const;
	sockaddr_in getSockaddrIPv4() const;
	sockaddr_in6 getSockaddrIPv6() const;
	std::string toString() const;
private:
	IPVersion ipversion = IPVersion::Unknown;
	std::string hostname = "";
	std::string ipString = "";
	std::vector<uint8_t> ipBytes = {};
	unsigned short port = 0;
};

std::ostream& operator<<(std::ostream& ostream, const IPEndpoint& endpoint);