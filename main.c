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
#include "../helpers.c"

int main(void) {
    init();
    print_num(0);
    while(1) {
        print_num(9);
        for (uint8_t led=0; led<2; led++) {
            // led 0, led 1 for loop
            print_num(led);
            print_num(9);
            for (uint8_t brightness=0; brightness<255; brightness++) {
                // fade up- 256 levels, 256ms fade time so f=1khz -> period=1ms
                blocking_pwm(brightness, 255, led);
            }
            print_num(2);
            for (uint8_t brightness=255; brightness>0; brightness--) {
                // fade down- 256 levels, 1000ms fade time so f=250 -> period=4ms
                blocking_pwm(brightness, 255, led);
            }
            print_num(3);

            
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