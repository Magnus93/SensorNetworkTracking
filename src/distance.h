#ifndef DISTANCE_H_
#define DISTANCE_H_

#include "settings.h"

void store_RSSI_value(int rssi_value);
void calculate_RSSI_average();
uint16_t calculate_distance();


#endif