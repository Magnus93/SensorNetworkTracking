#include "settings.h"

// returns the corresponding enum to the nodes address
// we will only use 4 nodes so only 8 bits of address is needed
Mote_t set_enum() {
	return linkaddr_node_addr.u8[0] - 1;
}

void init_mote() {
	mote = set_enum();

	char* enums[4] = {"Origin", "Yaxis", "Xaxis", "Sink"};
	printf("I am %s\n", enums[mote]);
}