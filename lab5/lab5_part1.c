/**
 *
 *
 *   Name:  Alex Neiman and Beck Dehlsen
 *   CPE 416
 *   Final project battle bot
 *      Task: Scan, seek, and pursue the closest object.
 *            Don't fall off the board.
 *
 **/

#include "../library/globals.h"
#include "lidar.h"
#include "proportional.h"

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <stdlib.h>

// hardware configuration
#define PIN_SENSOR_L    3
#define PIN_SENSOR_R    4
#define MOTOR_L         0
#define MOTOR_R         1

// error codes
#define BOUNDARY_WARNING 9000

// tuneable parameters (more in lidar.h and proportional.h)
#define LINE_THRESHOLD 150
#define TIMESTEP        100

// globals
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

// ISR for encoder
ISR(PCINT1_vect) {
   right_encoder++;  //increment right encoder
}



int main(void) {

    u08 sensor_pins[2] = {3,4}; // Analog pins that correspond to sensors to read
    u08 sensor_value[2];    // sensor values array
    int16_t target = 0;     // target angle (can)
    int8_t scan_dir=-1;     // robot scan direction
    motor_command mc;       // motor command
    int16_t initial_target, delta_angle, encoder_target;
    int16_t turning_direction;  // full scan direction

    // Initialize
    init();
    init_servo();
    init_encoder();
    stop_motor();
    point_servo(0);

    // wait for servos to settle
    clear_screen();
    lcd_cursor(0,0);print_string("BigFang");
    lcd_cursor(0,1);print_string("Bot4i");
    delayms(500);

    
    // Wait for button press before start
    while(!get_btn()) { _delay_ms(5); }
    while(get_btn()) { _delay_ms(5); }

    /* Pursue the target- main loop
        1. Do a full scan (compounding) with lidar and robot motion combined
        2. Point the robot in the direction of the closest object
        3. Drive forward
        4. [Loop] Repeat until line following sensor trips
            a. [Loop] Sweep lidar and determine angle of closest object
                i.  Update line sensors
                ii. Break out if line sensor trips
            b. Steer towards object (proportional ctl)
            c. Update line sensors
            d. Break out if line sensor trips
        5. Spin the robot towards the line to push the can off. Spin 180º
        6. Drive forward (towards center of board) to move away from line
        ... and repeat 
     */
    while(1) {

        // BIG SWEEP 
        // reset encoder
        right_encoder = 0;
        clear_screen();
        lcd_cursor(0,0);print_string("Seeking");
        // Do full scan
        initial_target = full_scan();
        // initial_target holds angle of closest can.
        clear_screen();lcd_cursor(0,0);print_string("Seeking");
        lcd_cursor(0,1);print_string("Locked");
        // turn towards the closest target
        // The bot finished its scan 90º to the right (30 ticks), so subtract 90º.
        delta_angle = initial_target - 90; // the relative angle from the robot to the target (signed)
        if (delta_angle > 0) {
            // target to the right - spin right and calculate num of encoder ticks
            encoder_target = delta_angle * (int16_t) 60 / (int16_t) 180;
            turning_direction = 1;
        }
        else {
            // target to the left- spin left (majority of cases)
            encoder_target = -1 * delta_angle * (int16_t) 60 / (int16_t) 180;
            turning_direction = -1;
        }
        // turn towards the target
        right_encoder = 0;
        while(right_encoder < encoder_target) {
            // turn based on direction
            motor(0, turning_direction * FWD_SPEED * 2);
            motor(1, turning_direction * -FWD_SPEED * 2);
            _delay_ms(10);
        }

        // Now travel straight forward (blindly) while the initial scan runs
        motor(MOTOR_L, FWD_SPEED);
        motor(MOTOR_R, FWD_SPEED);
        _delay_ms(10);
        
        // Repeated scan loop
        do {
            // localized lidar-only scan
            target = scan(scan_dir, LINE_THRESHOLD);
            // want lidar to sweep back and forth- swap scan direction
            scan_dir = -scan_dir;
            // update line sensors
            // Make sure this happens before any breakout of the loop so that the 
            for(u08 i=0;i<2;i++) {
                // Read ADC value
                sensor_value[i] = analog(sensor_pins[i]);;
            }
            // scan() will return BOUNDARY WARNING if it broke out early from
            // a scan due to a line sensor tripping during the loop.
            if (target == BOUNDARY_WARNING) {
                // If this is the case, stop the robot motion immediately and break out of the loop
                stop_motor();
                break;
            }
            // Otherwise:
            // Travel turning towards the target with this proportional gain (0.5)
            motor_dir(target * 50 / 100, &mc);
            motor(MOTOR_L, mc.left);
            motor(MOTOR_R, mc.right);
        // and if the sensor values exeed the threshold, break out of the loop
        } while (sensor_value[0] < LINE_THRESHOLD || sensor_value[1] < LINE_THRESHOLD);
        // Now check which sensor tripped the line threshold.
        // A negative turning_direction causes the left motor to reverse and right motor to go forward
        // so   -1 => CCW
        //      +1 => CW
        if (sensor_value[0] > LINE_THRESHOLD) {
            // left side of the robot touching line- so turn CCW
            clear_screen();
            lcd_cursor(0,0);print_string("Boundary");
            lcd_cursor(0,1);print_string("Warn (L)");
            turning_direction = -1;
        } else if (sensor_value[1] > LINE_THRESHOLD) {
            // right sensor line - turn CW
            clear_screen();
            lcd_cursor(0,0);
            lcd_cursor(0,0);print_string("Boundary");
            lcd_cursor(0,1);print_string("Warn (R)");
            turning_direction = 1;
        }
        // reset encoder and prepare to turn robot around 180º so that it is facing away from the line
        right_encoder = 0;
        encoder_target = 55;
        // turn around 180º
        while(right_encoder < encoder_target) {
            motor(MOTOR_L, turning_direction * FWD_SPEED * 2);
            motor(MOTOR_R, turning_direction * -FWD_SPEED * 2);
            _delay_ms(10);
        } 
        
        stop_motor();

        // update sensor readings proactively before we enter the loop
        for(u08 i=0;i<2;i++) {
            // Read ADC value
            sensor_value[i] = analog(sensor_pins[i]);;
        }
        // drive forward away from the line
        right_encoder = 0;
        encoder_target = 20;
        while(right_encoder < encoder_target || sensor_value[0] > LINE_THRESHOLD || sensor_value[1] > LINE_THRESHOLD) {
            motor(MOTOR_L, FWD_SPEED * 2);
            motor(MOTOR_R, FWD_SPEED * 2);
            _delay_ms(10);
            // We want this loop to break in case it runs near the line, so update the sensor readings again.
             for(u08 i=0;i<2;i++) {
                // Read ADC value
                sensor_value[i] = analog(sensor_pins[i]);;
            }
        }

        // Finally stop the motor so that we can do the main scan again.
        stop_motor();
    }

    // never reach this line
    return 0;
}

