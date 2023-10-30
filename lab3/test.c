

/**
 * 
 *
 *   Testing for neural net
 *
 **/


// Enable local- skip AVR commands and declarations
#define LOCAL

#include "../library/globals.h"
#include <math.h>
#include "proportional.h"
#include <stdlib.h>
#include "neuralnet.h"
#include "training_data.h"
#include <stdio.h>

#define SCREEN_SIZE_X 8
#define SCREEN_SIZE_Y 2
#define DELAY_TIME_BASE 1000
#define DELAY_TIME_MULTIPLIER 15 // ms per bit (accel output)
#define DEAD_ZONE 10 // multiply by 2 to get total dead zone
#define ACCEL_CENTER 128

#define PIN_SENSOR_L    3
#define PIN_SENSOR_R    4
#define MOTOR_L         0
#define MOTOR_R         1
#define MAX_TRAIN_DATA  100 // maximum number of training data points
#define TIMESTEP 10

#define DEBUG 1     // 0 -> normal run, 1 -> use DB vals
#define DB_EPOCHS 201

int main(void) {
    // Declarations
    nn network;
    u16 data_size = NUM_TRAINING_POINTS; // number of data points to train on [0, MAX_TRAIN_DATA]
    motor_command mc;
    u16 accel_read, dir_x;
    u16 epochs = 10000;

//    printf("W11,W12,B1,W21,W22,B2,W31,W32,B3,W41,W42,W43,B4,W51,W52,W53,B5\n");
    initialize_neural_network(&network); // set all weights to random
    train_neural_network(&network, data, data_size, epochs);

//    printf("LEARNING TEST\n\n");
    for (int i=0; i<NUM_TRAINING_POINTS; i++) {
        motor_command pd_mc = compute_proportional(noisy_data[i].left, noisy_data[i].right);
        motor_command ml_mc = compute_neural_network(noisy_data[i].left, noisy_data[i].right, &network);
        printf("%d, %d, %d, %d, %d, %d, %d, %d, %f\n", noisy_data[i].left, noisy_data[i].right, pd_mc.left, pd_mc.right, ml_mc.left, ml_mc.right, (pd_mc.left - pd_mc.right), (ml_mc.left - ml_mc.right), (float) ((ml_mc.left - ml_mc.right)-(pd_mc.left - pd_mc.right)) / (float) (pd_mc.left - pd_mc.right));
    }
    return 0;
}

