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

#define TX_POWER MID_TX_POWER
#define OFFSET 10		// the RSSI power indication when placing two motes on top of eachother (Cooja) 
#define RSSI_AMOUNT 10	// the number of RSSI readings to store in a buffer for the moving average


// Messages transmitted between nodes
#define REQUEST_AXIS 0
#define REQUEST_ORIGIN_DISTANCE 1
#define REQUEST_X_DISTANCE 2
#define REQUEST_Y_DISTANCE 3
#define REPLY_AXIS 5
#define REPLY_ORIGIN_DISTANCE 6
#define REPLY_X_DISTANCE 7
#define REPLY_Y_DISTANCE 8


typedef enum Mote
{	
	Nothing,
	Origin, 
	Yaxis, 
	Xaxis, 
	Sink
} Mote_t; 	// according to the addresses given by cooja

Mote_t mote;

uint8_t message;

void init_mote();
Mote_t get_type();

#endif