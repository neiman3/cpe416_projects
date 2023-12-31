#include <stdio.h>
#include <stdlib.h>
#include "neuralnet.h"

#define PIN_SENSOR_L    3
#define PIN_SENSOR_R    4
#define MOTOR_L         0
#define MOTOR_R         1
#define MAX_TRAIN_DATA  999 // maximum number of training data points

int main(void) {
    // Neural network model
    // Initialize neural network
    nn network;
    u08 sensor_l, sensor_r;
    u16 data_size; // number of data points to train on [0, MAX_TRAIN_DATA]
    sensor_reading data[MAX_TRAIN_DATA];    // left-right training data
    motor_command mc;

//    init();
    initialize_neural_network(&network); // set all weights to random
    train_neural_network(&network, data, MAX_TRAIN_DATA);
//    motor(MOTOR_L,0);
//    motor(MOTOR_R,0);
//    clear_screen();
    motor_command test = compute_neural_network(187,193, &network);
//    lcd_cursor(0,0); print_num(187);
//    lcd_cursor(4,0); print_num(193);
//    lcd_cursor(0,1); print_num((u16) test.left * 1000);
//    lcd_cursor(4,1); print_num((u16) test.right * 1000);
    return 0;

    print_string("Proportional");
    while(!get_btn()) {
        sensor_l = analog(PIN_SENSOR_L);
        sensor_r = analog(PIN_SENSOR_R);
        mc = compute_proportional(sensor_l, sensor_r);
        motor(MOTOR_L, mc.left);
        motor(MOTOR_R, mc.right);
//        _delay_ms(TIMESTEP);
    }

    clear_screen();
    lcd_cursor(5,0);
    print_string("Data");
    lcd_cursor(1,0);
    for(data_size=0; data_size<MAX_TRAIN_DATA && !get_btn(); data_size++) {
        sensor_l = analog(PIN_SENSOR_L);
        sensor_r = analog(PIN_SENSOR_R);
        data[data_size].left = sensor_l;
        data[data_size].right = sensor_r;
//        _delay_ms(TIMESTEP);
    }

    clear_screen();
    lcd_cursor(5,0);
    print_string("Train");
    lcd_cursor(1,0);
    // train_neural_network(nn_weights);

    //mc = compute_neural_network()

    return 0;
}

