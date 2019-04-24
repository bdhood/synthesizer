#include <stdlib.h>
#include <stdio.h>
#include "network/network.h"

int main() {
	#ifdef DEBUG
	printf("Debugging Mode Enabled\n");
	#endif
	#ifdef OS_WIN
	printf("Windows\n");
	#endif
	#ifdef OS_LINUX
	printf("Linux\n");
	#endif
	printf("its alive\n");
	
	NETWORK *net = network_create();

	return 0;
}