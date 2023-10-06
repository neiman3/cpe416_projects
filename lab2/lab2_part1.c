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

// call set_servo to spin the motor: 0 for left, 1 for right
// speed range: -100 to 100 -> 0 to 250
// speed = (speed + 100) * 250 / 200
void motor(uint8_t num, int8_t speed) {
    int32_t sp = ((int32_t) speed + 100) * 255 / 200;
    // map -100 to 100 to 0 to 250
    set_servo(0, sp);
    return;
}


int main(void) {

    init();  //initialize board hardware
    
    while (1) {
        for (int8_t i=0; i<=100; i++) {
            motor(0,i);
            motor(1,i);
            lcd_cursor(0,0);
            print_string("Speed:");
            print_num(i);
            _delay_ms(100);
        }
        for (int8_t i=100; i<=0; i++) {
            motor(0,i);
            motor(1,i);
            _delay_ms(100);
        }
        for (int8_t i=0; i<=-100; i++) {
            motor(0,i);
            motor(1,i);
            _delay_ms(100);
        }
        for (int8_t i=-100; i<=0; i++) {
            motor(0,i);
            motor(1,i);
            _delay_ms(100);
        }
    }

    return 0;
}

