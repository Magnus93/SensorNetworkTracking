#include "distance.h"


uint32_t x_size = 0;				// length of x axis
uint32_t y_size = 0;				// length of y axis
int rssi_readings[RSSI_AMOUNT];		// store the last readings to calculate a moving average (is EWA better?)
int rssi_index = 0;			// keeps track of the next location to store data in from rssi_readings
int avg_rssi = 0;
int accumulator = 0;



void store_RSSI_value(int rssi_value) {
	printf("rssi_index: %d\n", rssi_index);
	rssi_readings[rssi_index % RSSI_AMOUNT] = rssi_value + OFFSET;		// store the latest reading at the oldest value
	rssi_index++;
	if (rssi_index >= 100 * RSSI_AMOUNT) {rssi_index = RSSI_AMOUNT;}  // to not overflow, caution! dont set RSSI_AMOUNT to high!
	calculate_RSSI_average();
}

void set_axis(uint32_t x, uint32_t y) {
	x_size = x;
	y_size = y;
}

// Calculates the average RSSI value of the RSSI_AMOUNT latest packets
void calculate_RSSI_average() {
	accumulator = 0;
	if (rssi_index >= RSSI_AMOUNT) {
		int i;
		for (i = 0; i < RSSI_AMOUNT; ++i)
		{
			accumulator += rssi_readings[i];
		}
		avg_rssi = accumulator / RSSI_AMOUNT;
	} else {
		avg_rssi = 0;  // if we dont have enough readings for the moving average set the average to 0
	}
	printf("avg_rssi: %d \n", avg_rssi); 
	printf("accumulator: %d\n",  accumulator);
}

// Increase decimals for better resolution????
// See report for calculations of q
uint32_t calculate_distance() {
	int q = 0;				// quota * 10, (RSSI / DISTANCE) 
	uint32_t distance = 0;  // distance in cm
	switch (TX_POWER) {
		case MAX_TX_POWER :
		q = -16;
		break;

		case MID_TX_POWER :
		q = -27;
		break;

		case MIN_TX_POWER :
		q = -48;
		break;

		default :
		q = -16;
	}

	distance = ((avg_rssi * 100) / q) * 10;		// q is scaled 10x to be able to utilize ints
	printf("distance: %ldcm, q: %d\n", distance, q);
	return distance;
}

Position_t calculate_position(uint32_t x_dist, uint32_t y_dist, uint32_t o_dist) {
    uint32_t area, p, y_pos, x_pos;
    Position_t pos;
    p = (x_dist + x_size + o_dist) / 2;
    area = sqrt(p*(p-a)*(p-b)*(p-c));
    pos.yPos = 2 * area / x_size ; 
    pos.xPos = sqrt(o_dist^2 - y_pos^2);
    return pos;
}
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
