#pragma once
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>

class NetworkEnvironment
{
public:
	static bool initialize();
	static void shutDown();
};
