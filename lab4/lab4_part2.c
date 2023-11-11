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
#define LOCAL

#include "../library/globals.h"
#include <math.h>
#include "proportional.h"
#include "particle.h"
#ifndef LOCAL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#endif
#ifdef LOCAL
#include <stdio.h>
#endif

#define PIN_SENSOR_L    3
#define PIN_SENSOR_R    4
#define PIN_SENSOR_DIST 5
#define MOTOR_L         0
#define MOTOR_R         1
#define ACCEL_CENTER 127
#define DEAD_ZONE 20

#define TIMESTEP        10
#define UPDATE_INTV     100 // Time (ms) delay between simulation cycles


volatile uint16_t left_encoder = 0;
volatile uint16_t right_encoder = 0;

#ifndef LOCAL
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
    _delay_ms(TIMESTEP);
}

#endif


int main(void) {
    tower towers[MAX_NUM_TOWERS];
    // for (int i=0; i< MAX_NUM_TOWERS; i++){
    //     // reset all towers
    // }


    // hard code input data for now
    // Tower 1 at 15º
    // Tower 2 (vader) at 135º
    // Tower 3 at 180º
    // Tower 4 not enabled
    towers[0].position = float_to_fixed_point_pos(15);
    towers[0].active = 1;
    towers[0].target = 0;
    towers[1].position = float_to_fixed_point_pos(135);
    towers[1].active = 1;
    towers[1].target = 1;
    towers[2].position = float_to_fixed_point_pos(180);
    towers[2].active = 1;
    towers[2].target = 1;
    towers[3].position = 0;
    towers[3].active = 0;
    towers[3].target = 0;

    particle particles[NUM_PARTICLES]; // initialize array
    init_particles(particles, NUM_PARTICLES, towers, 3);

#ifndef LOCAL
    init();
    init_encoder();
    motor(MOTOR_L,0);
    motor(MOTOR_R,0);
    clear_screen();
#endif

#ifdef LOCAL
    // simulated
    float simulated_position = 47;
    uint16_t simulated_ticks = 0;
    float estimated_position;
    float estimated_position_confidence;
    while(1) {
        // advance robot position by 15  ticks (11º)
        simulated_ticks += 1;
        simulated_position += add_noise(simulated_ticks * 0.739, 1);
        simulated_position = wrap_degrees(simulated_position);
        motion_update(particles, NUM_PARTICLES, simulated_ticks);
        simulated_ticks = 0; // reset counter to get next delt

        // Take a simulated sensor reading
        uint8_t sensor_reading = (uint8_t) ((DIST_THRESHOLD_HIGH - DIST_THRESHOLD_LOW) * calculate_position_probability(simulated_position, towers, 3) + DIST_THRESHOLD_LOW);
        calculate_sensor_probability(sensor_reading, particles, NUM_PARTICLES, towers, 3);
        resample(particles, NUM_PARTICLES, towers, 3);
        mean_st_dev(particles, NUM_PARTICLES, &estimated_position, &estimated_position_confidence);
        printf("Actual position: (%3.2f)\tEstimated position: (%3.2f [%3.4f])\t", simulated_position, estimated_position, estimated_position_confidence);
        for (int i=0; i<NUM_PARTICLES; i++) {
            printf("%3.2f\t%1.3f\t", fixed_point_pos_to_float(particles[i].position), particles[i].weight);
        }
        printf("\n");

        // _delay_ms(TIMESTEP);
    }
#endif

    return 0;
}

