#ifndef SETTINGS
#define SETTINGS


#include "contiki.h"
#include "net/rime/rime.h"
#include <stdio.h>
#include "dev/cc2420/cc2420.h"
#include "distance.h"
// The three allowed TX powers
#define MAX_TX_POWER 31
#define MID_TX_POWER 19
#define MIN_TX_POWER 11

#define TX_POWER MID_TX_POWER
#define OFFSET 10		// the RSSI power indication when placing two motes on top of eachother (Cooja) 
#define RSSI_AMOUNT 10	// the number of RSSI readings to store in a buffer for the moving average

typedef enum Mote
{	
	Origin, 
	Yaxis, 
	Xaxis, 
	Sink
} Mote_t; 	// according to the addresses given by cooja

Mote_t mote = Origin;
#endif