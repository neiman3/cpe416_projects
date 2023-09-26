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

int main(void) {
    init();
    led_on(0);
    led_on(1);
    led_on(2);
    clear_screen();
    lcd_cursor(1,1);
    print_num(0);
    print_num(1);
    while(1) {}
}


/*

while true
    function blink(pin1)
    finction blink(pin2)


    --> blink(pin):
        for loop 0->255
            pwm brightness




*/
