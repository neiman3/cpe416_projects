/*/**
 * @file lab1_part1.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-25
 * 
 * @copyright Copyright (c) 2023
 * 
 * 
 * For part 1, write a program that gradually fades LED0 on and then off.  
 * Then, it should do the same with LED1 and then the program should repeat.  The fading effect should be smooth and without flicker.
 * 
 * You can make an LED appear partially on by turning the LED on and off in rapid succession.
 * 
 */

#include "../library/globals.h"
#include <util/delay.h>
#include <avr/io.h>
<<<<<<< HEAD
#include "helpers.c"
=======
#include "../helpers.c"
>>>>>>> 5db74da (clash)

void main() {
    init();
    while(1) {
<<<<<<< HEAD
        led_on(0);
        led_on(1);
        delay_ms(250);
        led_off(0);
        led_off(1);
        delay_ms(250);
=======
        for (uint8_t led=0; led<2; led++) {
            // led 0, led 1 for loop
            for (uint8_t brightness=0; brightness<256; brightness++) {
                // fade up- 256 levels, 256ms fade time so f=1khz -> period=1ms
                blocking_pwm(brightness, 1000, 0);
            }
            for (uint8_t brightness=255; brightness>0; brightness--) {
                // fade down- 256 levels, 1000ms fade time so f=250 -> period=4ms
                blocking_pwm(brightness, 4000, 1);
            }
            
            // delay between leds
            delay_ms(500);
        }
>>>>>>> 5db74da (clash)
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