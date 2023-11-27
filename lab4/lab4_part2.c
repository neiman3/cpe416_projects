/**
 *
 *
 *   Name:  Alex Neiman and Beck Dehlsen
 *   CPE 416
 *   Lab 3 Part 1: Proportional struct testing.
 *      Test the motor_command struct and the compute_proportional command
 *         ( wrapper for Lab2 in a single function )
 * 
 * TESTING ON ROBOT
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
#include "test_data.h"
#endif

#define PIN_SENSOR_L    3
#define PIN_SENSOR_R    4
#define PIN_SENSOR_DIST 5
#define MOTOR_L         0
#define MOTOR_R         1
#define ACCEL_CENTER 127
#define DEAD_ZONE 20

#define TICKS_UPDATE 10
#define TICKS_TO_DEG 1.33

#define TIMESTEP        10
#define UPDATE_INTV     100 // Time (ms) delay between simulation cycles

// #define DEBUG


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
}

void prompt_tower(tower *towers) {
    uint8_t target_sel = 0;
    // Get the towers from the user
    for (uint8_t t=0;t<MAX_NUM_TOWERS;t++) {
        int8_t accel_read_value;
        // each tower screen
        clear_screen();
        lcd_cursor(0,0); print_string("Tower "); print_num(t+1);
        lcd_cursor(0,1); print_string("[     ]");

        // Tower on or off
        lcd_cursor(2,1);
        print_string("Off");
        towers[t].active=0;
        while (!get_btn()) {
            if (read_accel(10) > 5) {
                // Turn that tower on
                lcd_cursor(2,1);
                print_string("On ");
                towers[t].active=1;
            }
            if (read_accel(10) < -5) {
                // Turn that tower off
                lcd_cursor(2,1);
                print_string("Off");
                towers[t].active=0;
            }
            _delay_ms(TIMESTEP);
        }
        while (get_btn()){}
        _delay_ms(10);

        if (!towers[t].active) {continue;} // it was not active, so don't set anything
        // Tower position (angle)
        uint16_t angle = (uint16_t) fixed_point_pos_to_float(towers[t].position);
        while (!get_btn()) {
            // Set tower position
            lcd_cursor(2,1);
            accel_read_value = read_accel(90);
            if (accel_read_value > 0) {
                if (angle < 360)
                    angle++; // 1 degree
            }
            if (accel_read_value < 0) {
                if (angle > 0)
                    angle--; // 1 degree
            }
            
            print_num(angle);
            // clear chars
            if ((uint16_t) angle < 100) {
                print_string(" ");
            }
            if ((uint16_t) angle  < 10) {
                print_string(" ");
            }
            for (uint8_t i=0; i<(100 - ((accel_read_value<0)?-accel_read_value:accel_read_value)); i++){
                _delay_ms(1);
                if (get_btn()) break;
            }
        }
        towers[t].position = float_to_fixed_point_pos(angle);
        while (get_btn()){}
        _delay_ms(10);

        // Set target
        lcd_cursor(0,1);
        print_string("Standard");
        towers[t].target=0;
        while (!get_btn() && (!target_sel)) {
            if (read_accel(10) > 5) {
                // Target
                lcd_cursor(0,1);
                print_string("[Target]");
                towers[t].target=1;
            }
            if (read_accel(10) < -5) {
                // Regular
                lcd_cursor(0,1);
                print_string("Standard");
                towers[t].target=0;
            }
            _delay_ms(TIMESTEP);
        }
        if (!target_sel)
            target_sel = towers[t].target;
        while (get_btn()){}
        clear_screen();
    }
}

#endif


int main(void) {


#ifdef LOCAL
    float simulated_position = 180;
#endif
    tower towers[MAX_NUM_TOWERS];
    towers[0].position = float_to_fixed_point_pos(0);
    towers[0].active = 1;
    towers[0].target = 0;
    towers[1].position = float_to_fixed_point_pos(45);
    towers[1].active = 1;
    towers[1].target = 0;
    towers[2].position = float_to_fixed_point_pos(90);
    towers[2].active = 1;
    towers[2].target = 0;
    towers[3].position = float_to_fixed_point_pos(225);
    towers[3].active = 0;
    towers[3].target = 0;

    // Calculate tower stats
    uint8_t num_towers = calc_num_towers(towers, MAX_NUM_TOWERS);
    float target_position = calc_target_tower(towers, num_towers);

    // Particles
    particle particles[NUM_PARTICLES]; // initialize array
    init_particles(particles, NUM_PARTICLES, towers, num_towers);


#ifndef LOCAL
    init();
    init_encoder();
    motor(MOTOR_L,0);
    motor(MOTOR_R,0);
    clear_screen(); lcd_cursor(0,0); print_string("Where");lcd_cursor(0,1); print_string("VaderBot");
    uint8_t skip_prompt = 0;
    for (uint8_t i=0;i<255;i++){
        _delay_ms(10);
        if (get_btn()) {
            skip_prompt = 1;
            break;
        }
    }


    #ifndef DEBUG
    if (!skip_prompt)
        prompt_tower(towers);
    #endif



    lcd_cursor(0,0); print_num(num_towers); print_string(" Towers");
    lcd_cursor(0,1);print_string("Tgt: ");print_num((uint16_t) target_position);print_string("  ");
    for (uint8_t i=0;i<255;i++){
        _delay_ms(10);
    }



    float estimated_position = 0;
    float estimated_position_stdev = 0;
    uint8_t sensor_reading = 1;
    uint8_t position_delta;
    right_encoder = 0;
    while(1) {

        // self-driving line following
        u08 sensor_l, sensor_r;
        sensor_l = analog(PIN_SENSOR_L);
        sensor_r = analog(PIN_SENSOR_R);
        sensor_reading = analog(PIN_SENSOR_DIST);
        motor_command mc = compute_proportional(sensor_l, sensor_r);
        motor(MOTOR_L, mc.left);
        motor(MOTOR_R, mc.right);


        if (right_encoder >= TICKS_UPDATE) {
            // time to update the particles
            // before the right encoder gets incremented, save it first
            position_delta = right_encoder;
            right_encoder = 0; // reset to get next delta.
            motion_update(particles, NUM_PARTICLES, position_delta);

            // Take a sensor reading
            calculate_sensor_probability(sensor_reading, particles, NUM_PARTICLES, towers, num_towers);
            resample(particles, NUM_PARTICLES, towers, num_towers);
            mean_st_dev(particles, NUM_PARTICLES, &estimated_position, &estimated_position_stdev);
            if (estimated_position_stdev < LOCALIZED_THRESHOLD) {
                // Done with localization
                break;
            }
        }
            // data dump
        lcd_cursor(0,0);print_string("p"); print_num((uint16_t) estimated_position);print_string("   ");
        lcd_cursor(4,0);print_string("s"); print_num((uint8_t) sensor_reading);print_string("   ");
        lcd_cursor(0,1);print_string("c"); print_num((uint8_t) estimated_position_stdev); print_string("."); print_num((uint16_t) (estimated_position_stdev * 1000));print_string("   ");
        lcd_cursor(6,1);print_string("w"); print_num(right_encoder);print_string("   ");
        _delay_ms(TIMESTEP);
    }

    // Target acquired- find tower
    clear_screen(); 
    lcd_cursor(0,0); print_string("T");print_num(target_position);
    lcd_cursor(0,1); print_string("E");print_num(estimated_position);
    float error;
    error = wrap_degrees(target_position - estimated_position);
    if (error > (360 - TARGET_WINDOW_DEGREES / 2)) {
        // within -10º
        error = 0;
    }
    if (error < TARGET_WINDOW_DEGREES / 2) {
        // within +10º
        error = 0;
    }
    lcd_cursor(4,1);print_string("X");print_num(error);

    
    uint16_t target_encoder_position = 0;
    target_encoder_position = (uint16_t) ((error) * TICKS_TO_DEG * 0.95);

    // Line follow until target
    u08 sensor_l, sensor_r;
    while (right_encoder < target_encoder_position) {
        sensor_l = analog(PIN_SENSOR_L);
        sensor_r = analog(PIN_SENSOR_R);
        sensor_reading = analog(PIN_SENSOR_DIST);
        motor_command mc = compute_proportional(sensor_l, sensor_r);
        motor(MOTOR_L, mc.left);
        motor(MOTOR_R, mc.right);
        _delay_ms(TIMESTEP);
        lcd_cursor(4,1);print_num(right_encoder);print_string("   ");
    }
    lcd_cursor(0,0);print_string("D");

    motor(MOTOR_L, 0);
    motor(MOTOR_R, 0);

    
    // Reached target, knock it over
    // reset encoder counter
    right_encoder = 0;
    // while encoder < 35 : left motor -40 right motor +40
    while (right_encoder < 32) {
        motor(MOTOR_L, -20);
        motor(MOTOR_R, 20);
    }
    motor(MOTOR_L, 0);
    motor(MOTOR_R, 0);
    _delay_ms(100);
    motor(MOTOR_L, -90);
    motor(MOTOR_R, -90);
    _delay_ms(255);
    _delay_ms(255);
    _delay_ms(255);

    motor(MOTOR_L, 0);
    motor(MOTOR_R, 0);
    // stop motor
    // motor +40 +40
    // delay for 1 second
    // stop motor
    while(1) {}

    

#endif

#ifdef LOCAL
    // simulated
    
    uint16_t simulated_ticks = 0;
    float estimated_position;
    float estimated_position_confidence;
    uint8_t sensor_reading;

    // print number of towers for Python script
    for (int i=0; i< num_towers; i++){
        printf("%d\t", (uint16_t) fixed_point_pos_to_float(towers[i].position));
    }printf("\n");

    printf("Actual position\tEstimated position\tParticle StDev\tSensor\t");
    for (int i=0;i<NUM_PARTICLES;i++) {
        printf("Particle %d location\tParticle %d weight\t", i,i);
    }printf("\n");
//    for (int b=0; b<360; b+=10) {
//        init_particles(particles, NUM_PARTICLES, towers, 3);
//        simulated_position = (float) b;
        uint16_t i;
        for (i = 0; i < 100; i++) {
            // advance robot position by 15  ticks (11º)
            simulated_ticks += 10;
            simulated_position += add_noise(simulated_ticks * 0.739, 0.1);
            simulated_position = wrap_degrees(simulated_position);
            motion_update(particles, NUM_PARTICLES, simulated_ticks);
            simulated_ticks = 0; // reset counter to get next delt

            // Take a simulated sensor reading
            sensor_reading = (uint8_t) ((DIST_THRESHOLD_HIGH - DIST_THRESHOLD_LOW) *
                                        calculate_position_probability(simulated_position, towers, num_towers) / 0.1 +
                                        DIST_THRESHOLD_LOW);
            calculate_sensor_probability(sensor_reading, particles, NUM_PARTICLES, towers, num_towers);
            resample(particles, NUM_PARTICLES, towers, num_towers);
            mean_st_dev(particles, NUM_PARTICLES, &estimated_position, &estimated_position_confidence);

            // if (estimated_position_confidence < LOCALIZED_THRESHOLD) {
            //     // localized
            //     break;
            // }

            // Weights dump
                    printf("%3.2f\t%3.2f\t%3.4f\t%d\t", simulated_position, estimated_position, estimated_position_confidence, sensor_reading);
                    for (int i=0; i<NUM_PARTICLES; i++) {
                        printf("%3.2f\t%1.3f\t", fixed_point_pos_to_float(particles[i].position), particles[i].weight);
                    }
                    printf("\n");

            // _delay_ms(TIMESTEP);
        }
//        printf("%3.1f, %3.1f, %3.1f, %0.3f, %d\n", (float) b, simulated_position, estimated_position, estimated_position_confidence, i);
//    }
#endif

    return 0;
}

