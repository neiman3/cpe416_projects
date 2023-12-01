

// #include "neuralnet.h"
#include "proportional.h"
// #define LOCAL


/**
 *
 *
 *   Name:  Alex Neiman and Beck Dehlsen
 *   CPE 416
 *   Lab 3 ANN training basis (can calculate target/training values)
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

motor_command compute_proportional(u08 left_value, u08 right_value) {

    u08 sensor_value[2] = {left_value, right_value};
    int8_t output[2];

    // control variables
    int16_t theta_deg = 0; // proportiona
    // int16_t theta_deg_d = 0; // derivative
    // int16_t theta_deg_i = 0; // integral

    u08 VWL_set = VWL;
    u08 VWR_set = VWR;
    u08 VBL_set = VBL;
    u08 VBR_set = VBR;
    u08 VSTATE_B_B_set = VSTATE_B_B;
    u08 VSTATE_W_W_set = VSTATE_W_W;



    float theta;
    // bound
    sensor_value[0] = bound(sensor_value[0], VWL_set, 255);
    sensor_value[1] = bound(sensor_value[1], VWR_set, 255);


    // calculate 2d mapping transformation of angular difference of sensor reading
    theta = calculate_theta(sensor_value[0], VWL_set, sensor_value[1], VWR_set);
    theta_deg = proportional_error(theta, THETA_FWD);
    // theta_deg_i = integral_error(theta_deg_history, THETA_FWD);

    // calculate vstate vector magnitude to determine black, white, etc
    float vstate;
    vstate = calculate_vstate_vector(VBL_set, sensor_value[0], VBR_set, sensor_value[1]);


    if(vstate < VSTATE_B_B_set) {
        // black on black or tape crossing
        // go forward blindly
        // OUTPUT MOTOR DIR 0
        #ifndef LOCAL
        motor_dir(0, output);
        #endif

    } else if (vstate > VSTATE_W_W_set) {    // outside threshold for proportional control - 1 value too low (black-white case)
        //        // neither sensor reading the tape (white-white case)
        // spin in the direction of side_last_found

        // OUTPUT MOTOR DIR SPIN LEFT ONLY
        output[0] = 0;
        output[1] = FWD_SPEED;

    } else {
        motor_dir(((int16_t) theta_deg) * GAIN_KP_NUM / GAIN_KP_DEN, output);
    }
    motor_command result = {output[0], output[1]};
    return result;
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