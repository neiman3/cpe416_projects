#include "../library/globals.h"
#include <math.h>
#include "proportional.h"
#define SWEEP_ANGLE_MIN -127
#define SWEEP_ANGLE_MAX 127
#define NUM_TARGETS 7

#define PIN_SENSOR_DIST 5
#define BOUNDARY_WARNING 9000

#define SCAN_ANGLESTEP 1
#define SCAN_TIMESTEP 6
#define SCAN_ANGLE_WIDTH 20

#define TICKS_TO_90_DEGREES_CONV 25

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>


// Board object model
typedef struct
{
    uint16_t theta;
    uint8_t magnitude;
} object;

int16_t scan(int8_t scan_direction, uint8_t threshold_value);
int16_t full_scan();
void point_servo(int16_t angle);
int16_t settle(int16_t target, int8_t *scan_dir);
int16_t calibrate_angle(int16_t angle, uint8_t distance);
void print_signed(int16_t value);
void wait_for_servo(int16_t current, int16_t target);
void rotate_robot(uint16_t target, int8_t direction);
int16_t compound_angle_calculation(int16_t servo_position, int16_t encoder_value);
