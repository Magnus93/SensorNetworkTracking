#ifndef DISTANCE_H_
#define DISTANCE_H_

#include "settings.h"

void store_RSSI_value(int rssi_value, Mote_t mote);
void calculate_RSSI_average(Mote_t mote);
uint16_t calculate_distance(Mote_t mote);
void set_axis(uint16_t x, uint16_t y);

#endif