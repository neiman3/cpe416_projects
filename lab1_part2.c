/*/**
 * @file lab1_part2.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-09-25
 * 
 * @copyright Copyright (c) 2023
 * 
 * 
 * Write a program that repeatedly scrolls your full name across the LCD display.  
 * Pressing the user button should switch to the name of your lab partner.  
 * Your program should run continuously.
 * 
 */

#include "./library/globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "helpers.c"

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