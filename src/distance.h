#ifndef DISTANCE_H_
#define DISTANCE_H_

#include "settings.h"
#include <math.h>
void store_RSSI_value(int rssi_value);
void calculate_RSSI_average();
uint32_t calculate_distance();
void set_axis(uint32_t x, uint32_t y);
Position_t calculate_position(uint32_t x_dist, uint32_t y_dist, uint32_t o_dist);

#endif
