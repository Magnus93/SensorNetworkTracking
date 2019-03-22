#include "settings.h"

// returns the corresponding enum to the nodes address
// we will only use 4 nodes so only 8 bits of address is needed
Mote_t set_type() {
	return linkaddr_node_addr.u8[0];
}

void init_mote() {
	mote = set_type();

	char* enums[5] = {"Nothing", "Origin", "Yaxis", "Xaxis", "Sink"};
	printf("I am %s\n", enums[mote]);
}

Mote_t get_type() {
	return mote;
}