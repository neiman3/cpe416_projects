

// #include "neuralnet.h"
#include "proportional.h"
// #define LOCAL


/**
 *
 *
 *   Name:  Alex Neiman and Beck Dehlsen
 *   CPE 416
 *   Lab 5 Proportional control library
 *
 **/


float calculate_vstate_vector(u08 vbl_set, u08 sensor_0, u08 vbr_set, u08 sensor_1) {
    return (float) sqrt((double) (vbl_set - sensor_0) * (vbl_set - sensor_0) + (double) (vbr_set - sensor_1) * (vbr_set - sensor_1));
}

float calculate_theta(u08 sensor_0, u08 vwl_set, u08 sensor_1, u08 vwr_set) {
    return atanf((float) (sensor_0 - vwl_set) / (float) (sensor_1 - vwr_set)) * 180 / 3.14;
}


int16_t min(int16_t arg1, int16_t arg2) {
    if (arg1 < arg2){
        return arg1;
    }
    return arg2;
}

int16_t max(int16_t arg1, int16_t arg2) {
    if (arg1 > arg2){
        return arg1;
    }
    return arg2;
}

int16_t bound(int16_t arg, int16_t minimum, int16_t maximum) {

    return max(minimum, min(maximum, arg));
}

int16_t abs(int16_t val) {
    if (val > 0)
        return val;
    return -val;
}



void motor_dir(int16_t angle, motor_command *output) {
    // takes  a signed 8 bit "angle" +- 200 deg
    // robot is turning with two radii so it's not really an angle...
    // debugging mode = if button pressed, disable motor
    angle = bound(angle, -100, 100);

    int16_t power_l = FWD_SPEED;
    int16_t power_r = FWD_SPEED;

    if (angle < 0) {
        angle = angle * -1;
        // turn left, cut power to left motor and boost power to right motor
        power_l -= (angle * FWD_SPEED / 100);
        power_r += (angle * FWD_SPEED / 100);
    }
    else if (angle > 0) {
        // turn right, cut power to right motor and boost power to left motor
        power_l += (angle * FWD_SPEED / 100);
        power_r -= (angle * FWD_SPEED / 100);
    }
    power_l = bound(power_l, 0, 100);
    power_r = bound(power_r, 0, 100);

    output->left = power_l;
    output->right = power_r;
    return;
}
#ifndef LOCAL
void motor(uint8_t num, int8_t speed) {
    int32_t sp = ((int32_t) speed * SERVO_CAL / 200) + 127;
    if (num == 1) { // selected first wheel
        // reverse right wheel
        set_servo(num, 255 - sp - 1);
    } else {
        set_servo(num, sp);
    }
    return;
}

#endif


void time_advance(u16 *array, u16 new_value) {
    // 0 being most recent
    // positive index meaning negative time index (in the past/previous])
    for (u08 i =  (HISTORY_LENGTH - 1); i > 0; i--) {
        array[i] = array[i-1];
    }
    array[0] = new_value;
}

int16_t proportional_error(u16 value, u16 setpoint) {
    return setpoint - value;
}

int16_t integral_error(u16 *array, u16 setpoint) {
    // return integral of (array minus error)
    int16_t result = 0;
    for (u08 i=0; i<HISTORY_LENGTH; i++) {
        result += (setpoint - array[i]);
    }
    result = result / HISTORY_LENGTH;
    return result;

}

int16_t derivative_error(u16 *array, u16 setpoint) {
    // return time derivative of (array minus error)
    return ((int16_t) setpoint - array[0]) - ((int16_t) setpoint - array[1]);
}

uint8_t map_float_to_servo_int(float input_value) {
    // FWD_SPEED should be 0.5 (default), with off beign 0 and full speed being 1
    return bound((uint8_t) (input_value * 2 * FWD_SPEED), 0, 2* FWD_SPEED);
}

float map_servo_int_to_float(uint8_t input_value) {
    // FWD_SPEED should be 0.5 (default), with off beign 0 and full speed being 1
    return ((float) input_value / (2 * FWD_SPEED));
}


float map_sensor_reading_to_float(u08 left) { return (float) (left - VWL) / (VBL - VWL); }

void stop_motor() {
    motor(0,0);
    motor(1,0);
    _delay_ms(10);
}