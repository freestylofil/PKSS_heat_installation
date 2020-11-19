#pragma once
#include "IncludeMe.h"
#include "Client.h"
#include <vector>
#include <json.h>

#define NO_TIMEENDPOINT
#define NO_ENERGY_SUPPLIER
#define NO_HEAT_EXCHANGER
#define NO_BUILDING_0
#define NO_BUILDING_1
#define NO_BUILDING_2
#define NO_BUILDING_3

class Network
{
public:
	Network();
	void joinUnicastClients();
	~Network();
public:
	Server server;
#ifndef NO_TIMEENDPOINT
	Client timeClient;
#endif
#ifndef NO_ENERGY_SUPPLIER
	Client energySupplierClient;
#endif
#ifndef NO_HEAT_EXCHANGER
	Client heatExchangerClient;
#endif
	std::vector<Client> buildingClients;
private:
	void connectUnicastClients();
private:
	const IPEndpoint serverEndpoint = { "192.168.192.100", 4790 };
#ifndef NO_TIMEENDPOINT
	const IPEndpoint timeEndpoint = { "192.168.192.253", 8123 };
#endif
#ifndef NO_ENERGY_SUPPLIER
	const IPEndpoint energySupplierEndpoint = { "192.168.192.15", 8080 };
#endif
#ifndef NO_HEAT_EXCHANGER
	const IPEndpoint heatExchangerEndpoint = { "192.168.192.123", 43516 };
#endif
	const std::array<IPEndpoint, 4> buildingEndpoints = { 
#ifndef NO_BUILDING_0
																IPEndpoint{"192.168.192.111", 51111},
#endif
#ifndef NO_BUILDING_1
																IPEndpoint{"192.168.192.12", 65042},
#endif
#ifndef NO_BUILDING_2
																IPEndpoint{"192.168.192.82", 50621},
#endif
#ifndef NO_BUILDING_3
																IPEndpoint{"192.168.192.91", ????}
#endif
	};
};

