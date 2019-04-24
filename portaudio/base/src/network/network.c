#include <stdlib.h>
#include "network.h"

NETWORK *network_create() {
	NETWORK *network = malloc(sizeof(NETWORK));
	network->servers = network_servers_load();
	return network;
}