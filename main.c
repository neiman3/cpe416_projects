/**
 * 
 *
 *   Name:  Alex Neiman and Beck Dehlsen
 *   CPE 416
 *   Lab 2 part 1
 *
 *   Description: Spin motors back and forth while displaying speed
 *
 **/


#include "../library/globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define SERVO_SPEED_CTL_GAIN 3 // One over the gain (divide by this)
#define TIMESTEP 20

// call set_servo to spin the motor: 0 for left, 1 for right
// speed range: -100 to 100 -> 0 to 250
// speed = (speed + 100) * 250 / 200
// gain
void motor(uint8_t num, int8_t speed) {
    int32_t sp = ((int32_t) speed) / SERVO_SPEED_CTL_GAIN * 255 / 200 + 128;
    if (num == 1) { // selected first wheel
        // reverse right wheel
        set_servo(num, 255 - sp + 1);
    } else {
        set_servo(num, sp);
    }
    return;
}


int main(void) {

    init();  //initialize board hardware
    
    while (1) {
        for (int8_t i=0; i<=50; i++) {
            motor(0,i);
            motor(1,i);
            lcd_cursor(0,0);
            clear_screen();
            print_string("s1:");
            print_num(i);
            _delay_ms(TIMESTEP);
        }
        for (int8_t i=50; i>=0; i--) {
            motor(0,i);
            motor(1,i);
            clear_screen();
            print_string("s2:");
            print_num(i);
            _delay_ms(TIMESTEP);
        }
        for (int8_t i=0; i>=-50; i--) {
            motor(0,i);
            motor(1,i);
            clear_screen();
            print_string("s3:");
            print_num(i);
            _delay_ms(TIMESTEP);
        }
        for (int8_t i=-50; i<=0; i++) {
            motor(0,i);
            motor(1,i);
            clear_screen();
            print_string("s4:");
            print_num(i);
            _delay_ms(TIMESTEP);
        }
    }

    return 0;
}

