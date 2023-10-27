/**
 * 
 *
 *   Name:  Alex Neiman and Beck Dehlsen
 *   CPE 416
 *   Lab 3 Part 2: Neural network line following
 *
 **/

#include "../library/globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include "proportional.h"
#include <stdlib.h>
#include "neuralnet.h"

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
    u08 sensor_l, sensor_r;
    u16 data_size; // number of data points to train on [0, MAX_TRAIN_DATA]
    sensor_reading data[MAX_TRAIN_DATA];    // left-right training data
    motor_command mc;
    u16 accel_read, dir_x;
    u16 epochs = (MAX_EPOCHS + MIN_EPOCHS)/2;
    // Initialize 
    init();

    motor(MOTOR_L,0);
    motor(MOTOR_R,0);
    clear_screen(); print_string("Press to"); lcd_cursor(0,1); print_string("run");
    while(!get_btn()) {}
    while(get_btn()) _delay_ms(50);
    
    // Demo mode- proportional control
    if(!DEBUG) {
        lcd_cursor(0,0); print_string("Proportional");
        while(!get_btn()) {
            sensor_l = analog(PIN_SENSOR_L);
            sensor_r = analog(PIN_SENSOR_R);
            mc = compute_proportional(sensor_l, sensor_r);
            motor(MOTOR_L, mc.left);
            motor(MOTOR_R, mc.right);
        _delay_ms(TIMESTEP);
        }
        while(get_btn()) _delay_ms(50);
    }


    // Data collection mode
    clear_screen();
    lcd_cursor(0,0);
    print_string("Data");
    motor(MOTOR_L,0);
    motor(MOTOR_R,0);
    for(data_size=0; data_size<MAX_TRAIN_DATA && !get_btn(); data_size++) {
        sensor_l = analog(PIN_SENSOR_L);
        sensor_r = analog(PIN_SENSOR_R);
        data[data_size].left = sensor_l;
        data[data_size].right = sensor_r;
        lcd_cursor(0,1);
        print_num(sensor_l);
        lcd_cursor(5,1);
        print_num(sensor_r);
        lcd_cursor(5,0); print_num(data_size);
        
       _delay_ms(TIMESTEP*5);
    }
    while(get_btn()) _delay_ms(50);
    clear_screen();

    if (DEBUG) {
        for (u16 i=0; i<data_size;i++){
            clear_screen();
            lcd_cursor(0,0);
            print_num(data[i].left);
            lcd_cursor(0,1);
            print_num(i);
            lcd_cursor(4,0);
            print_num(data[i].right);
            _delay_ms(100);

        }
    }

    // Main loop
    while(1) {
       // Select number of epochs 
        if(!DEBUG) {
            lcd_cursor(0,0);
            print_string("Train");
            motor(MOTOR_L, 0);
            motor(MOTOR_R, 0);
            while(!get_btn()) {
                accel_read = 128 - get_accel_y();
                if (accel_read < (ACCEL_CENTER - DEAD_ZONE)) {
                    // its pointing in negative direction
                    dir_x = -1 + 0*(accel_read - (ACCEL_CENTER - DEAD_ZONE));
                } else if (accel_read > (ACCEL_CENTER + DEAD_ZONE)) {
                    // pointing in positive
                    dir_x = 1 + 0*(accel_read > (ACCEL_CENTER + DEAD_ZONE));
                } else {
                    // it's in the dead zone
                    dir_x = 0;
                }
                epochs += dir_x;
                epochs = min(max(MIN_EPOCHS, epochs), MAX_EPOCHS);
                lcd_cursor(0,1);
                print_num(epochs);

                _delay_ms(TIMESTEP);
            }
        } else {
            epochs = DB_EPOCHS;
        }

        // debounce
        while(get_btn()) _delay_ms(50);


        // Run training algorithm
        lcd_cursor(0,0); print_string("Training");
        initialize_neural_network(&network); // set all weights to random
        train_neural_network(&network, data, data_size, epochs);

        // Run ML line following mode
        clear_screen();
        while(!get_btn()) {
            sensor_l = analog(PIN_SENSOR_L); lcd_cursor(0,0); print_num(sensor_l);
            sensor_r = analog(PIN_SENSOR_R); lcd_cursor(4,0); print_num(sensor_r);
            mc = compute_neural_network(sensor_l, sensor_r, &network);
            motor(MOTOR_L, mc.left); lcd_cursor(0,1); print_num(mc.left);
            motor(MOTOR_R, mc.right);lcd_cursor(4,1); print_num(mc.right);
            _delay_ms(TIMESTEP);
        }
        while(get_btn()) _delay_ms(50);
    }
    return 0;
}

