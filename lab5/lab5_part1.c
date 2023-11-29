/**
 *
 *
 *   Name:  Alex Neiman and Beck Dehlsen
 *   CPE 416
 *   Final project
 *
 **/

#include "../library/globals.h"
#include <math.h>
#include "proportional.h"
#include "lidar.h"

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define PIN_SENSOR_L    3
#define PIN_SENSOR_R    4
#define MOTOR_L         0
#define MOTOR_R         1
#define ACCEL_CENTER 127
#define DEAD_ZONE 20
#define DIST_THRESHOLD_LOW 38
#define DIST_THRESHOLD_HIGH 100

#define TICKS_UPDATE 10

#define TIMESTEP        100

volatile uint16_t right_encoder = 0;

// Rotary encoder ISR setup
void init_encoder() {
    // enable encoder interrupts
    EIMSK = 0;
    EIMSK |= _BV(PCIE1) | _BV(PCIE0);
    PCMSK1 |= _BV(PCINT13); //PB5 - digital 5
    PCMSK0 |= _BV(PCINT6);  //PE6 - digital 4
    // enable pullups
    PORTE |= _BV(PE6);
    PORTB |= _BV(PB5);
}

ISR(PCINT1_vect) {
   right_encoder++;  //increment right encoder
}


int8_t read_accel(uint8_t scale) {
    // return value on a scale of 0 to (scale)
    int8_t dir_x;
    uint8_t accel_read = 128 - get_accel_y();
    if (accel_read < (ACCEL_CENTER - DEAD_ZONE)) {
        // its pointing in negative direction
        dir_x = (accel_read - (ACCEL_CENTER - DEAD_ZONE));
    } else if (accel_read > (ACCEL_CENTER + DEAD_ZONE)) {
        // pointing in positive
        dir_x = (accel_read - (ACCEL_CENTER + DEAD_ZONE));
    } else {
        // it's in the dead zone
        dir_x = 0;
    }
    return dir_x * scale / 38;
}

void sensor_test() {
    u08 sensor_l, sensor_r;
    sensor_l = analog(PIN_SENSOR_L);
    sensor_r = analog(PIN_SENSOR_R);
    motor_command mc = compute_proportional(sensor_l, sensor_r);
    motor(MOTOR_L, mc.left);
    motor(MOTOR_R, mc.right);
    // right encoder test
    lcd_cursor(0,0);print_num(right_encoder);print_string("    ");
    lcd_cursor(0,1);print_num(read_accel(10));print_string("    ");
    lcd_cursor(4,0);print_num(analog(PIN_SENSOR_DIST));print_string("    ");
}



int main(void) {

    // Initialize
    init();
    init_servo();
    init_encoder();
    motor(MOTOR_L,0);
    motor(MOTOR_R,0);
    clear_screen();

    int16_t target;
    int8_t scan_dir=1;
    uint8_t board_edge_flag = 0;
    int8_t angle;
    motor_command mc;

    point_servo(SWEEP_ANGLE_MIN);
    wait_for_servo(SWEEP_ANGLE_MIN, SWEEP_ANGLE_MAX);

    while(1) {
        while (1) {
            target = full_scan(scan_dir, 1, target);
            scan_dir = -scan_dir;
            lcd_cursor(0,0);print_signed(target);print_string("   ");
            motor_dir(target * 50 / 100, &mc);
            lcd_cursor(4,0);print_num(mc.left);
            lcd_cursor(4,1);print_num(mc.right);
            motor(MOTOR_L, mc.left);
            motor(MOTOR_R, mc.right);

        }

        // Given angle of target, determine left and right motor power (prop. control)
        mc.left = 50;
        mc.right = 50;

        

        if (board_edge_flag) {
            // turn away from board edge
        }

        _delay_ms(TIMESTEP);
    }

    return 0;
}

