#include "Network.h"
#include "Logger.h"

Network::Network()
{
	NetworkEnvironment::initialize();
	server.start(serverEndpoint);
	
	connectUnicastClients();
	#ifndef NO_TIME
	timeClient.bindToServer(timeEndpoint);
	#endif
	joinUnicastClients();
}

void Network::joinUnicastClients()
{
#ifndef NO_ENERGY_SUPPLIER
	energySupplierClient.join();
#endif
#ifndef NO_HEAT_EXCHANGER
	heatExchangerClient.join();
#endif
	for (Client& client : buildingClients)
	{
		client.join();
	}
}

Network::~Network()
{
	NetworkEnvironment::shutDown();
}

void Network::connectUnicastClients()
{
#ifndef NO_ENERGY_SUPPLIER
	energySupplierClient.connectToServer(energySupplierEndpoint);
#endif
#ifndef NO_HEAT_EXCHANGER
	heatExchangerClient.connectToServer(heatExchangerEndpoint);
#endif
	for (std::size_t i = 0; i < buildingEndpoints.size(); ++i)
	{
		buildingClients.push_back(Client());
		buildingClients[i].connectToServer(buildingEndpoints[i]);
	}
}
