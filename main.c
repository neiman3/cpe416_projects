/*/**
 * @file lab1_part4.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-09-25
 *
 * @copyright Copyright (c) 2023
 *
 *
 * Create a program which scrolls the number ’416’ on the display that is 
 * controlled by the tilt of the board.  The program should work along the 2 axes.  
 * The number should never completely disappear from the screen, but should show at least 1 digit.
 *
 */

#include "./library/globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#define SCREEN_SIZE_X 7
#define SCREEN_SIZE_Y 2

#define DELAY_TIME_BASE 1000
#define DELAY_TIME_MULTIPLIER 15 // ms per bit (accel output)
#define DEAD_ZONE 10 // multiply by 2 to get total dead zone
#define ACCEL_CENTER 128

void print_string_wrapping(char *string, u08 base_pos_x, u08 base_pos_y) {
    u08 pos_x = 0;
    u08 pos_y = 0;
    // prints left to right
    // will wrap around a scrolling text that exceeds the screen size
    clear_screen();
    u08 num_chars = strlen(string);
    for (int i=0; i<num_chars; i++){
        pos_x = base_pos_x + i;
        pos_y = base_pos_y;
        //each character
        while (pos_x >= SCREEN_SIZE_X) {
            pos_x = pos_x - SCREEN_SIZE_X;
        }
        while (pos_y >= SCREEN_SIZE_Y) {
            pos_y = pos_y - SCREEN_SIZE_Y;
        }
        lcd_cursor(pos_x, pos_y);
        print_string(string+i);
    }
    return;
}

int main(void) {
    u08 pos_x = 0; //  position x value
    u08 pos_y = 0; //  position y value
    u08 accel_read;
    int8_t dir_x;
    int8_t dir_y;
    u16 timer, update_time_x, update_time_y;
    u08 update = 0;    

    char display_text[25];
    char msg[] = "416";

    timer = 0;
    update_time_x = 0;
    update_time_y = 0;
    
    init();  //initialize board hardware

    print_string_wrapping("Hello", 4, 0);

    while(1) {
        //DEBUG

        // make the math easy- add constant offset 128 to accel data so that 128 = 0
        // magnitude of acceleration is the absolute value |128 - accel|
        // That of course means greater magnitude = shorter delay time
        if (timer > update_time_x) {
            // read accel yes it's y axis for x
            accel_read = 128 - get_accel_y();
            // calculate direction
            if (accel_read < (ACCEL_CENTER - DEAD_ZONE)) {
                // its pointing in negative direction
                dir_x = -1;
            } else if (accel_read > (ACCEL_CENTER + DEAD_ZONE)) 
            {
                // pointing in positive
                dir_x = 1;
            } else {
                // it's in the dead zone
                dir_x = 0;
            }

            update_time_x = timer + (DELAY_TIME_BASE - DELAY_TIME_MULTIPLIER * dir_x * (accel_read - 128));
            // calculate magnitude and update update time x
            
            // update pos x based on direction
            pos_x = pos_x + dir_x;
            update = 1;

        }
        if (timer > update_time_y) {
            // Make a visually pleasing simple motion for y
            // read accel yes it's y axis for x
            accel_read = 128 - get_accel_x();
            // calculate direction
            if (accel_read < (ACCEL_CENTER - DEAD_ZONE)) {
                // its pointing in top direction
                pos_y = 0;
            } else if (accel_read > (ACCEL_CENTER + DEAD_ZONE)) 
            {
                // pointing down
                pos_y = 1;
            } else {
                // it's in the dead zone- leave unchanged
            }

            update_time_y = timer + (DELAY_TIME_BASE - DELAY_TIME_MULTIPLIER * dir_x * (accel_read - 128));
            // calculate magnitude and update update time x
            
            // update pos x based on direction
            update = 1;

        }

        if (update) {
            update = 0;
            print_string_wrapping(msg, pos_x, pos_y);
        }

        _delay_ms(1);
        timer++;
    }

    return 0;
}