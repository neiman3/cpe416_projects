/**
 * 
 *
 *   Name:  Alex Neiman and Beck Dehlsen
 *   CPE 416
 *   Lab 3 Part 1: Proportional struct testing. 
 *      Test the motor_command struct and the compute_proportional command
 *         ( wrapper for Lab2 in a single function )
 *
 **/


#include "../library/globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include "proportional.h"

#define PIN_SENSOR_L    3
#define PIN_SENSOR_R    4
#define PIN_SENSOR_DIST 5
#define MOTOR_L         0
#define MOTOR_R         1
#define ACCEL_CENTER 127
#define DEAD_ZONE 20

#define TIMESTEP        10

#define MAX_NUM_TOWERS  4

typedef struct
{
    uint16_t position;
    uint8_t target;
    uint8_t active;
} tower;

volatile uint16_t left_encoder = 0;
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

ISR(PCINT0_vect) {
   left_encoder++;  //increment left encoder
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
    sensor_l = analog(PIN_SENSOR_L);
    sensor_r = analog(PIN_SENSOR_R);
    motor_command mc = compute_proportional(sensor_l, sensor_r);
    // motor(MOTOR_L, mc.left);
    // motor(MOTOR_R, mc.right);
    // right encoder test
    // motor(MOTOR_R, 20);
    lcd_cursor(0,0);print_num(right_encoder);print_string("    ");
    lcd_cursor(0,1);print_num(read_accel(10));print_string("    ");
    lcd_cursor(4,0);print_num(analog(PIN_SENSOR_DIST));print_string("    ");
    _delay_ms(TIMESTEP);
}

int main(void) {

    u08 sensor_l, sensor_r;
    tower towers[MAX_NUM_TOWERS];
    // for (int i=0; i< MAX_NUM_TOWERS; i++){
    //     // reset all towers
    // }


    // hard code input data for now
    // Tower 1 at 15º
    // Tower 2 (vader) at 135º
    // Tower 3 at 180º
    // Tower 4 not enabled
    tower[0].position = 15;
    tower[0].active = 1;
    tower[0].target = 0;
    tower[1].position = 135;
    tower[1].active = 1;
    tower[1].target = 1;
    tower[2].position = 180;
    tower[2].active = 1;
    tower[2].target = 1;
    tower[3].position = 0;
    tower[3].active = 0;
    tower[3].target = 0;
    


    init();
    init_encoder();
    motor(MOTOR_L,0);
    motor(MOTOR_R,0);
    clear_screen(); 

    while(1) {


        
    }

   return 0;
}

