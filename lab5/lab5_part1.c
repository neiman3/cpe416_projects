/**
 *
 *
 *   Name:  Alex Neiman and Beck Dehlsen
 *   CPE 416
 *   Final project battle bot
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
#define LINE_THRESHOLD 150
#define BOUNDARY_WARNING 9000

#define TICKS_UPDATE 10

#define TIMESTEP        100

#define DEBUG

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


int main(void) {
    u08 sensor_pins[2] = {3,4}; // Analog pins that correspond to sensors to read
    u08 sensor_value[2]; // sensor values array

    // Initialize
    init();
    init_servo();
    init_encoder();
    stop_motor();
    clear_screen();

    // while (!get_btn()) {
    //     for(u08 i=0;i<2;i++) {
    //         // Read ADC value
    //         sensor_value[i] = analog(sensor_pins[i]);
    //     }
    //     for(u08 i=0;i<2;i++) {
    //         // Read ADC value
    //         lcd_cursor(4*i, 0);print_num(sensor_value[i]);print_string("   ");
    //     }
    //     _delay_ms(10);
    // }

    clear_screen();
    lcd_cursor(0,0);print_string("BigFang");
    lcd_cursor(0,1);print_string("Bot4i");

    for (uint8_t i=0;i<255;i++){
        _delay_ms(4);
    }

    int16_t target = 0;
    int8_t scan_dir=-1;
    motor_command mc;
    int16_t initial_target, delta_angle, encoder_target;
    int8_t turning_direction;

    point_servo(0);
    clear_screen();
    lcd_cursor(0,0);print_string("Zero Deg");

    while(!get_btn()) {
        //wait
    }

    while(get_btn()) {
        // wait
    }

    clear_screen();
    lcd_cursor(0,1);print_string("ScanTest");

    while(!get_btn()) {
        lcd_cursor(0,0);
        print_signed(target);print_string("       ");
        target = scan(scan_dir, LINE_THRESHOLD);
        _delay_ms(10);
        _delay_ms(10);
        _delay_ms(10);
        _delay_ms(10);
        _delay_ms(10);
        scan_dir = -scan_dir;
        
    }
    

    /* Pursue the target */
    while(1) {

        // Do initial sweep
        right_encoder = 0;

        clear_screen();
        lcd_cursor(0,0);print_string("Seeking");
 
        initial_target = full_scan();

        clear_screen();lcd_cursor(0,0);print_string("Seeking");
        lcd_cursor(0,1);print_string("Locked");
        // lcd_cursor(0,0);
        // print_signed(initial_target);
        // turn from +30 encoder ticks to target
        delta_angle = initial_target - 90; // the relative angle from the robot to the target (signed)
        if (delta_angle > 0) {
            // target to the right
            encoder_target = delta_angle * (int16_t) 60 / (int16_t) 180;
            turning_direction = 1;
        }
        else {
            encoder_target = -1 * delta_angle * (int16_t) 60 / (int16_t) 180;
            turning_direction = -1;
        }
        // lcd_cursor(4,1);
        // print_signed(encoder_target);
        // lcd_cursor(0,1);
        // print_signed(turning_direction);

        /* Find the initial target */
        right_encoder = 0;
        while(right_encoder < encoder_target) {
            // clear_screen();
            // lcd_cursor(4,0);print_num(encoder_target);print_string("   ");
            // turn based on direction
            motor(0, turning_direction * FWD_SPEED * 2);
            motor(1, turning_direction * -FWD_SPEED * 2);
            _delay_ms(10);
        }
        motor(MOTOR_L, FWD_SPEED);
        motor(MOTOR_R, FWD_SPEED);
        _delay_ms(10);
        
        do {
            target = scan(scan_dir, LINE_THRESHOLD);
            scan_dir = -scan_dir;

            // read line sensors
            for(u08 i=0;i<2;i++) {
                // Read ADC value
                sensor_value[i] = analog(sensor_pins[i]);;
            }

            if (target == BOUNDARY_WARNING) {
                stop_motor();
                break;
            }

            motor_dir(target * 50 / 100, &mc);
            motor(MOTOR_L, mc.left);
            motor(MOTOR_R, mc.right);

        } while (sensor_value[0] < LINE_THRESHOLD || sensor_value[1] < LINE_THRESHOLD);

        clear_screen();
        lcd_cursor(4,0);print_num(sensor_value[0]);
        lcd_cursor(4,1);print_num(sensor_value[1]);
            if (sensor_value[0] > LINE_THRESHOLD) {
                // left sensor line - turn CW
                clear_screen();
                lcd_cursor(0,0);print_string("Boundary");
                lcd_cursor(0,1);print_string("Warn (L)");

                turning_direction = 1;
            } else if (sensor_value[1] > LINE_THRESHOLD) {
                // right sensor line - turn CCW
                clear_screen();
                lcd_cursor(0,0);
                lcd_cursor(0,0);print_string("Boundary");
                lcd_cursor(0,1);print_string("Warn (R)");
                turning_direction = -1;
            }
            // backpedal for a bit
            // motor(MOTOR_L, -FWD_SPEED);
            // motor(MOTOR_R, -FWD_SPEED);
            // _delay_ms(200);
            right_encoder = 0;
            encoder_target = 60;

            // turn around 180º

            while(right_encoder < encoder_target) {
                motor(MOTOR_L, turning_direction * FWD_SPEED * 2);
                motor(MOTOR_R, turning_direction * -FWD_SPEED * 2);
                _delay_ms(10);
            } 
            
            stop_motor();

            // update sensor
            for(u08 i=0;i<2;i++) {
                // Read ADC value
                sensor_value[i] = analog(sensor_pins[i]);;
            }

            right_encoder = 0;
            encoder_target = 20;
            while(right_encoder < encoder_target || sensor_value[0] > LINE_THRESHOLD || sensor_value[1] > LINE_THRESHOLD) {
                motor(MOTOR_L, FWD_SPEED);
                motor(MOTOR_R, FWD_SPEED);
                _delay_ms(10);
            }
            stop_motor();
    }

    return 0;
}

