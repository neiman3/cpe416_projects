/**
 * 
 *
 *   Name:  Alex Neiman and Beck Dehlsen
 *   CPE 416
 *   Lab 3 ANN training
 *
 **/


#include "../../library/globals.h"
#include <math.h>

// local machine only
#include <stdio.h>


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

#define FWD_SPEED 30 // Default forward speed should be 30
#define THETA_FWD 45 // straight angle (zero/forward)
#define HISTORY_LENGTH 10
#define TIMESTEP 100


typedef struct {
    int8_t left;
    int8_t right;
} motor_command;

// motor_command proportional(u08 , u08);


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



void motor_dir(int16_t angle, int8_t *output) {
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

    output[0] = power_l;
    output[1] = power_r;
    return;
}


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
    u08 side_last_found = 0; // which sensor read tape most recently - for black-white case. 0 means left

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
        motor_dir(0, output);
        
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




int main(void) {

    // float nn_weights[NUM_WEIGHTS];   // Weights from neural net training




    motor_command mc = compute_proportional(193, 182);
    
    // // init weights pre-training
    // for(int i=0; i<NUM_WEIGHTS; i++) {
    //     nn_weights = 0;
    //     nn_weights = -1;    // bias "weights"
    // }

    // train model
    // train_nn(nn_weights);

    printf("%d", mc.left); printf(" ");
    printf("%d", mc.right);
    printf("\n");


    return 0;
}