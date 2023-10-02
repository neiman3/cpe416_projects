/*/**
 * 

Name:  Alex Neiman and Beck Dehlsen
Lab 1 part 2

Description: Text scrolling with string manipulation and non-blocking program flow for AVR MCU

 */

#include "../library/globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "./helpers.c"


#define DELAY_TIME_MS 350
#define PIN_BUTTON 0
/*
    read button to pick string, loop thru chars in string with delay
*/

void update_text(u08 user_number, char* result) {
    // result is a  25 char string
    switch (user_number)
    {
    case 0:
        strcpy(result, "       Alex Neiman       ");
        break;
    
    case 1:
        strcpy(result, "       Beck Dehlsen      ");
        break;
    
    default:
        strcpy(result, "error                    ");
    }
}

int main(void) {

    // start with base string
    // we would want an array of strings (all possible names/display texts) that we could iterate through
    // OR a function that just contains a switch case statement that can write string based on an index value

    // working variables
    char text[25] = "       Hello world       "; // 24 chars
    uint8_t button_value;

    // flags and couters
    u16 timer = 0; //inidialite timer to zero
    u08 scroll_position = 0;
    uint8_t update_flag = 1;
    uint8_t button_press_flag = 0;
    uint8_t user_selection = 0;

    // any init here
    init();
    update_text(user_selection, text);

    while(1) { // MAIN LOOP- each pass takes 1 ms aka 1khz update rate

        timer++; // increment the timer
        
        if (timer >= DELAY_TIME_MS) { // Timer exceeded
            // reset timer and flag for an update of the LCD
            timer = 0;
            scroll_position++;
            if (scroll_position > 17) {
                scroll_position = 0;
            }
            update_flag = 1;
        }



        button_value = get_btn();
        if (button_value) {
            // button was pressed
            if (!button_press_flag) {
                button_press_flag = 1;
                update_flag = 1;
                user_selection++;
                if(user_selection>1) {
                    user_selection = 0;
                }
                update_text(user_selection, text);
            }
        } else {
            if (button_press_flag) {
                // user has let go of the button
                button_press_flag = 0;
                delay_ms(50);
            }
        }


        if (update_flag) {
            // do all grapics update here
            // slice the string and display
            // get string from user name variables
            lcd_cursor(0,0);
            print_string(text + scroll_position);
            update_flag = 0;

        }
        _delay_ms(1);
    }

    
}

