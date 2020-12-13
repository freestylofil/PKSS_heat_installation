#include "Network.h"
#include "Logger.h"

Network::Network()
	: 
#ifndef NO_SERVER
	server(TransmissionType::unicast, 500ul),
#endif
#ifndef NO_TIME
	  timeClient(timeServerEndpoint, TransmissionType::broadcast, 500ul),
#endif
#ifndef NO_ENERGY_SUPPLIER
	  energySupplierClient(energySupplierServerEndpoint, TransmissionType::unicast, 500ul),
#endif
#ifndef NO_HEAT_EXCHANGER
	  heatExchangerClient(heatExchangerServerEndpoint, TransmissionType::unicast, 500ul),
#endif
#ifndef NO_LOGGER
	loggingClient(loggingServerEndpoint, TransmissionType::unicast, 500ul),
#endif
#if N_BUILDINGS > 0
	  buildingClients{Client(buildingServerEndpoints[0], TransmissionType::unicast, 500ul),
	                  Client(buildingServerEndpoints[1], TransmissionType::unicast, 500ul),
	                  Client(buildingServerEndpoints[2], TransmissionType::unicast, 500ul),
	                  Client(buildingServerEndpoints[3], TransmissionType::unicast, 500ul) 
}
#endif
{
	NetworkEnvironment::initialize();
	Json defaultServerData;
	defaultServerData["Fzm"] = 200.0;
	defaultServerData["Fzco"] = 200.0;
	defaultServerData["Tpco"] = 20.0;
#ifndef NO_SERVER
	server.start(serverEndpoint, defaultServerData);
#endif
	
	#ifndef NO_TIME
	timeClient.bind(timeEndpoint);
	timeClient.join();
	#endif

	connectUnicastClients();
	
	joinClients();
#ifndef NO_LOGGER
	loggingClient.join();
#endif
}

void Network::joinClients()
{
#ifndef NO_TIME
	timeClient.join();
#endif
#ifndef NO_ENERGY_SUPPLIER
	energySupplierClient.join();
#endif
#ifndef NO_HEAT_EXCHANGER
	heatExchangerClient.join();
#endif
#if N_BUILDINGS > 0
	for (Client& client : buildingClients)
	{
		client.join();
	}
#endif
}

Network::~Network()
{
	joinClients();
	NetworkEnvironment::shutDown();
}

void Network::connectUnicastClients()
{
#ifndef NO_ENERGY_SUPPLIER
	energySupplierClient.connectToServer();
#endif
#ifndef NO_HEAT_EXCHANGER
	heatExchangerClient.connectToServer();
#endif
#ifndef NO_LOGGER
	loggingClient.connectToServer();
#endif
#if N_BUILDINGS > 0
	for (std::size_t i = 0; i < N_BUILDINGS; ++i)
	{
		buildingClients[i].connectToServer();
	}
#endif
}
