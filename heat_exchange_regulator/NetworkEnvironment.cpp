#include "NetworkEnvironment.h"
#include "NetworkException.h"
#include <iostream>

void NetworkEnvironment::initialize()
{
    WORD versionRequested = MAKEWORD(2, 2);
    WSADATA wsadata;
    if (WSAStartup(versionRequested, &wsadata) != 0)
    {
        int errorCode = WSAGetLastError();
        throw NETWORK_EXCEPTION(errorCode);
    }

    if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2)
    {
        throw std::runtime_error("Could not find a usable version of the winsock api dll");
    }
}

void NetworkEnvironment::shutDown()
{
    if (WSACleanup() != 0)
    {
        int errorCode = WSAGetLastError();
        throw NETWORK_EXCEPTION(errorCode);
    }
}

