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
 */

#include "../library/globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "helpers.c"


#define DELAY_TIME_MS 2000
#define PIN_BUTTON 0
/*
    read button to pick string, loop thru chars in string with delay
*/

int main(void) {
    init();

    // start with base string
    // we would want an array of strings (all possible names/display texts) that we could iterate through
    // OR a function that just contains a switch case statement that can write string based on an index value

    // working variables
    uint8_t text[14] = "Hello world";
    uint8_t slice[7];
    uint8_t button_value;

    // flags and couters
    u16 timer = 0; //inidialite timer to zero
    uint8_t update_flag = 1;
    uint8_t button_press_flag = 0;
    uint8_t user_selection = 0;


    while(1) { // MAIN LOOP- each pass takes 1 ms aka 1khz update rate

        timer++; // increment the timer
        
        if (timer >= DELAY_TIME_MS) { // Timer exceeded
            // reset timer and flag for an update of the LCD
            timer = 0;
            update_flag = 1;
        }



        button_value = get_btn();
        if (button_value) {
            // button was pressed
            if (!button_press_flag) {
                // user has just pressed the button
                button_press_flag = 1;
                update_flag = 1;
            }
        } else {
            if (button_press_flag) {
                // user has let go of the button
                button_press_flag = 0;
                delay_ms(50);
            }
        }


        if (update_flag) {
            // slice the string and display
            // get string from user name variables
            // slice_string(*text, *slice, 0, 7);
            user_selection++;
            lcd_cursor(0,0);
            print_num(user_selection);
            print_string("   ");
            lcd_cursor(0,1);
            print_num(timer);
            print_string("   ");
            // print_string(*slice);
            update_flag = 0;

        }
        _delay_ms(1);
    }

    
}

