#include "../library/globals.h"
#include <math.h>
#include "proportional.h"
#define SWEEP_ANGLE_MIN -127
#define SWEEP_ANGLE_MAX 127
#define NUM_TARGETS 7

#define PIN_SENSOR_DIST 5

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>


// Board object model
typedef struct
{
    uint16_t theta;
    uint8_t magnitude;
} object;

int16_t full_scan(int8_t scan_direction, uint8_t full_sweep, int16_t init_angle);
void point_servo(int16_t angle);
int16_t settle(int16_t target, int8_t *scan_dir);
int16_t calibrate_angle(int16_t angle, uint8_t distance);
void print_signed(int16_t value);
void wait_for_servo(int16_t current, int16_t target);
