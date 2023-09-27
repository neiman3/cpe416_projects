/*/**
 * 

Name:  Alex Neiman and Beck Dehlsen
Lab 1 part 3

Description: Pong game with LED blinking

 */

#include "../library/globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "helpers.c"


#define PIN_BUTTON 5
#define BUTTON_SPAM_REJECT_TIME 50 // time, in ms, when a repeat button press will be ignored
/*
    read button to pick string, loop thru chars in string with delay
*/


int main(void) {
    // Init
    init();
    // io configuration- input for button, output for leds

    // start with base string
    // we would want an array of strings (all possible names/display texts) that we could iterate through
    // OR a function that just contains a switch case statement that can write string based on an index value

    // working variables
    uint8_t button_value;
    u16 button_last_pressed = 0;

    u16 timer = 0; //inidialite timer to zero
    u08 delay_time = 255; // delay time in ms, max 255
    u16 time_next_led_update = timer + delay_time;

    u08 game_position = 0; // 0 to 4, 0 being the right most
                            // 0-51 first LED, 52-102, 103-153, 154,-204, 205-255 each directly addressing the output pins D0-D3
    u08 game_direction = 0; // 0 going left, 1 going right
    uint8_t update_flag = 1;
    uint8_t button_press_flag = 0;


    while(1) { // MAIN LOOP- each pass takes 1 ms aka 1khz update rate

        timer++; // increment the timer
        



        button_value = get_btn();
        if (button_value) {
            // button was pressed
            if ((!button_press_flag) && ((button_last_pressed + BUTTON_SPAM_REJECT_TIME) < timer)) {
                // All button press event stuff go here
                button_press_flag = 1;
                update_flag = 1;
                
                button_last_pressed = timer;

                if (check_direction(game_position, game_direction)) {
                // if the LED is at one of the extents, set the direction to opposite (invert directrion)
                    direction = !direction;
                }

            }
        } else if (button_press_flag) {
            // user has let go of the button
            button_press_flag = 0;
        }
        
        /*if (update_flag) {
            // do all grapics update here
            // slice the string and display
            // get string from user name variables
            lcd_cursor(0,0);
            // Write to the new IO pins (button on)
            // Turn off other IO pins
        }*/
        
        if(timer >= time_next_led_update || update_flag) {
            // LED position update here
            time_next_led_update = timer + delay_time;
            // Turn off old LED
            // Turn on new LED
            display_position(game_position);
            // reset update flag
            update_flag=0;

        }
        
        _delay_ms(1);
    }

    while(1) {
        // end game state
        // Display 
        // all LED off
    }

}



int check_direction(u08 current_position, u08 current_direction) {
    // Check position and direction
    // If it returns 1, it's a valid condition for the user to switch directions
    // if it returns zero, fon't change direction
    switch (current_direction)
    {
    case 0: // traveling left
        if (current_position == 5) {
            return 1;
        }
    case 1:
        if (current_position == 0) {
            return 1;
        }
    
    default:
        break;
    }
    return 0;
}

void display_position(u08 led) {
    // 0-4 will turn on JUST that IO pin and turn off the other ones
    // 5 will turn them all off
    // 6 will turn them all on
    u08 led_override=0;
    if (led==6) {
        led_override = 1;
    }
    for (u08 pin=0; pin<4; pin++) {
        // turn on the LED pin if the current pin is equal to the serlected display position
        // or, if they all should be on, turn on every LED
        digital_out(pin, ((pin==led) || led_override));
    }
    return;
}

void init_io() {
    // 0-4 set up as output
    // pin 5 set up as button input
    for (u08 pin=0; pin<4; pin++) {
        // turn on the LED pin if the current pin is equal to the serlected display position
        // or, if they all should be on, turn on every LED
        digital_dir(pin, 1); // 1 mode output
    }
    digital_dir(PIN_BUTTON, 0); // button pin input
    return;
}