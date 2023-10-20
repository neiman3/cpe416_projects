/**
 * 
 *
 *   Name:  Alex Neiman and Beck Dehlsen
 *   CPE 416
 *   Lab 3 Part 2: Neural network line following
 *
 **/

#include "./library/globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdlib.h>
#include "proportional.h"

#define PIN_SENSOR_L    3
#define PIN_SENSOR_R    4
#define MOTOR_L         0
#define MOTOR_R         1
#define TIMESTEP        100

#define NUM_WEIGHTS = 17    // number of weights in the NN model, including biases
#define EPOCHS = 1000       // full training cycles

// iterate on training data, adjusting weights after each epoch
void train_neural_network(float *weights, float *data) {
    return;
}

// scale sensor inputs, then calculate a motor command using neural net
motor_command compute_neural_network(u08 left, u08 right) {
    return;
}

int main(void) {

    float nn_weights[NUM_WEIGHTS];   // Weights from neural net training
    u08 sensor_l, sensor_r;

    init();

    motor(MOTOR_L,0);
    motor(MOTOR_R,0);
    clear_screen();
    lcd_cursor(0,0);
    print_string("Proportional");

    // proportional control
    while(!get_btn()) {
        sensor_l = analog(PIN_SENSOR_L);
        sensor_r = analog(PIN_SENSOR_R);
        motor_command mc = compute_proportional(sensor_l, sensor_r);
        motor(MOTOR_L, mc.left);
        motor(MOTOR_R, mc.right);
        _delay_ms(TIMESTEP);
    }

    clear_screen();
    lcd_cursor(0,0);
    print_string("Data");

    motor_command mc = compute_proportional(193, 182);
    
     // init weights pre-training
    for(int i=0; i<NUM_WEIGHTS; i++) {
        nn_weights = rand() / RAND_MAX;
    }

    // train model
    train_neural_network(nn_weights);

    printf("%d", mc.left); printf(" ");
    printf("%d", mc.right);
    printf("\n");


    return 0;
}