#include "NetworkEnvironment.h"
#include <iostream>

bool NetworkEnvironment::initialize()
{
    WORD versionRequested = MAKEWORD(2, 2);
    WSADATA wsadata;
    if (WSAStartup(versionRequested, &wsadata) != 0)
    {
        std::cerr << "Failed to start up winsock API.";
        return false;
    }

    if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2)
    {
        std::cerr << "Could not find a usable version of the winsock api dll.";
        return false;
    }

    return true;
}

void NetworkEnvironment::shutDown()
{
    WSACleanup();
}

