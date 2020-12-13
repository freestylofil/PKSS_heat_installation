#pragma once
#include "IncludeMe.h"
#include "Client.h"
#include <vector>
#include <array>

//#define NO_SERVER
//#define NO_TIME
//#define NO_ENERGY_SUPPLIER
//#define NO_HEAT_EXCHANGER
//#define NO_LOGGER
//#define NO_BUILDING_RADEK
//#define NO_BUILDING_KASIA
//#define NO_BUILDING_JAKUB
//#define NO_BUILDING_MATEUSZ
#define N_BUILDINGS 4


class Network
{
private:
#ifndef NO_SERVER
	const IPEndpoint serverEndpoint = { "192.168.192.100", 4790 };
#endif
#ifndef NO_TIME
	const IPEndpoint timeEndpoint = { "192.168.192.100", 8123 };
	const IPEndpoint timeServerEndpoint = { "192.168.192.253", 8123 };
#endif
#ifndef NO_ENERGY_SUPPLIER
	IPEndpoint energySupplierServerEndpoint = { "192.168.192.15", 8080 };
#endif
#ifndef NO_HEAT_EXCHANGER
	const IPEndpoint heatExchangerServerEndpoint = { "192.168.192.123", 43516 };
#endif
#ifndef NO_LOGGER
	const IPEndpoint loggingServerEndpoint = { "192.168.192.120", 55555 };
#endif
#if N_BUILDINGS > 0
	const std::array<IPEndpoint, N_BUILDINGS> buildingServerEndpoints = {
#ifndef NO_BUILDING_RADEK
																IPEndpoint{"192.168.192.111", 51111},
#endif
#ifndef NO_BUILDING_KASIA
																IPEndpoint{"192.168.192.12", 52345},
#endif
#ifndef NO_BUILDING_JAKUB
																IPEndpoint{"192.168.192.82", 50621},
#endif
#ifndef NO_BUILDING_MATEUSZ
																IPEndpoint{"192.168.192.2", 8051}
#endif
	};
#endif
public:
	Network();
	void joinClients();
	~Network();
public:
#ifndef NO_SERVER
	Server server;
#endif
#ifndef NO_TIME
	Client timeClient;
#endif
#ifndef NO_ENERGY_SUPPLIER
	Client energySupplierClient;
#endif
#ifndef NO_HEAT_EXCHANGER
	Client heatExchangerClient;
#endif
#ifndef NO_LOGGER
	Client loggingClient;
#endif
#if N_BUILDINGS > 0
	std::array<Client, N_BUILDINGS> buildingClients;
#endif
private:
	void connectUnicastClients();
};

