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
#include <avr/interrupt.h>

#include "../helpers.c"

/*
    read button to pick string, loop thru chars in string with delay
*/

void main() {
    uint8_t text[14] = "Hello world";
    uint8_t slice[7];
    while(true) {
        slice_string(*text, *slice, 0, 7)
    }
}


// This function should take pointer to two string and modify them in memory accordingly.
// Need to check the syntax for that- as the following ois not correct
void slice_string(*base_string, *destination_string, uint8_t index, uint8_t num_chars) {
    for (character=0; character<(num_chars); character++){
        destination_string[character] = base_string[character + index];
    }
}