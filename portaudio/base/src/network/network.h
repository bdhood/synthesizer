#ifndef __NETWORK_H

typedef struct SERVER {
	char *ipv4;
	char *ipv6;
	char *key;
	int port;
} SERVER;

typedef struct NETWORK {
	SERVER *servers;
} NETWORK;

NETWORK *network_create();
SERVER *network_servers_load();
#endif