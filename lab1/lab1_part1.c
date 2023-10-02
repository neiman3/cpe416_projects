/*/**
 * 

Name:  Alex Neiman and Beck Dehlsen
Lab 1 part 2

Description: LED PWM control for AVR MCU- blinks one LED at a time using builtin LED IO control from bublebee library

 */

#include "../library/globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include "./helpers.c"

int main(void) {
    init();
    print_string("Lab 1");
    lcd_cursor(0,1);
    print_string("Part 1");
    while(1) {
        for (uint8_t led=0; led<2; led++) {
            // led 0, led 1 for loop
            for (uint8_t brightness=0; brightness<255; brightness++) {
                // fade up- 256 levels, 256ms fade time so f=1khz -> period=1ms
                blocking_pwm(brightness, 10, led);
            }
            for (uint8_t brightness=255; brightness>0; brightness--) {
                // fade down- 256 levels, 1000ms fade time so f=250 -> period=4ms
                blocking_pwm(brightness, 40, led);
            }
            // delay between leds
            delay_ms(500);
        }
    }
}


/*

while true
    function blink(pin1)
    finction blink(pin2)


    --> blink(pin):
        for loop 0->255
            pwm brightness




*/