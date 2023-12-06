/**
 *
 *
 *   Name:  Alex Neiman and Beck Dehlsen
 *   CPE 416
 *   Final project battle bot
 *      Task: Scan, seek, and pursue the closest object.
 *            Don't fall off the board.
 *
 **/

#ifndef LOCAL_V2_PROPORTIONAL_H
#define LOCAL_V2_PROPORTIONAL_H

#include "../library/globals.h"
#include <math.h>

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Tunings and threhsold
#define VWL 170 // white threshold for left
#define VWR 170 // white threshold for right
#define VBL 212 // black threshold for left
#define VBR 212 // black threshold for right
#define VSTATE_W_W 50 // white on white Vstate threshold
#define VSTATE_B_B 10 // black on black threshold

#define GAIN_KP_NUM 25
#define GAIN_KP_DEN 10

#define GAIN_KI_NUM 10
#define GAIN_KI_DEN 10

#define GAIN_KD_NUM 10
#define GAIN_KD_DEN 10

#define FWD_SPEED 60 // Default forward speed should be 30
#define THETA_FWD 45 // straight angle (zero/forward)
#define HISTORY_LENGTH 10
#define SERVO_CAL 40

typedef struct {
    int8_t left;
    int8_t right;

} motor_command;

typedef struct {
    uint8_t left;
    uint8_t right;
} sensor_reading;

void motor_dir(int16_t angle, motor_command *output);
void motor(uint8_t num, int8_t speed);
void time_advance(u16 *array, u16 new_value);
float calculate_vstate_vector(u08 vbl_set, u08 sensor_0, u08 vbr_set, u08 sensor_1);
float calculate_theta(u08 sensor_0, u08 vwl_set, u08 sensor_1, u08 vwr_set);
int16_t min(int16_t arg1, int16_t arg2);
int16_t max(int16_t arg1, int16_t arg2);
int16_t bound(int16_t arg, int16_t minimum, int16_t maximum);
int16_t abs(int16_t val);
int16_t proportional_error(u16 value, u16 setpoint);
int16_t integral_error(u16 *array, u16 setpoint);
int16_t derivative_error(u16 *array, u16 setpoint);
motor_command compute_proportional(u08 left_value, u08 right_value);
void debug(char *string, u16 value);
uint8_t map_float_to_servo_int(float input_value);
float map_servo_int_to_float(uint8_t input_value);
float map_sensor_reading_to_float(u08 left);
void stop_motor();


#endif //LOCAL_V2_PROPORTIONAL_H
