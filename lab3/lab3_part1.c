#include "../library/globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include "proportional.h"

#define PIN_SENSOR_L    3
#define PIN_SENSOR_R    4
#define MOTOR_L         0
#define MOTOR_R         1

#define TIMESTEP        10

#define NUM_WEIGHTS = 17    // number of weights in the NN model, including biases
#define EPOCHS = 1000       // full training cycles


int main(void) {

    u08 sensor_l, sensor_r;

    init();
    motor(MOTOR_L,0);
    motor(MOTOR_R,0);
    clear_screen(); lcd_cursor(0,0); print_string("Test");lcd_cursor(0,1);print_string("Mode");

    while(1) {
        sensor_l = analog(PIN_SENSOR_L);
        sensor_r = analog(PIN_SENSOR_R);
        motor_command mc = compute_proportional(sensor_l, sensor_r);
        motor(MOTOR_L, mc.left);
        motor(MOTOR_R, mc.right);
        _delay_ms(TIMESTEP);
    }

   return 0;
}

