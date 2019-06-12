#ifndef SETTINGS_H_
#define SETTINGS_H_


#include "contiki.h"
#include "net/rime/rime.h"
#include <stdio.h>
#include "dev/cc2420/cc2420.h"

// The three allowed TX powers
#define MAX_TX_POWER 31
#define MID_TX_POWER 19
#define MIN_TX_POWER 11

#define TX_POWER 	MID_TX_POWER
#define OFFSET 		10		// the RSSI power indication when placing two motes on top of eachother (Cooja) 
#define RSSI_AMOUNT 1		// the number of RSSI readings to store in a buffer for the moving average


// Messages transmitted between nodes
#define REQUEST_AXIS 	 0
#define REQUEST_DISTANCE 1
#define REPLY_AXIS 		 2
#define REPLY_DISTANCE 	 3

// Locations in the data packet
#define COMMAND  		 0
#define DISTANCE 		 1
#define WAIT_FOR_COMMAND 2

typedef enum Mote
{	
	Nothing,
	Origin, 
	Yaxis, 
	Xaxis, 
	Sink
} Mote_t; 	// according to the addresses given by cooja

typedef struct Position {
	uint32_t xPos;
	uint32_t yPos;
} Position_t;

typedef struct Distances {
	uint32_t origin;
	uint32_t xaxis;
	uint32_t yaxis;
} Distances_t;

Mote_t mote;

void init_mote();
Mote_t get_type();

#endif