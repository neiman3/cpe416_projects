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


// calculate starting index of msg using get_accel funcs
u08 get_msg_position(u08 msg_size) {
    u08 pos;

    return pos;
}

void print_string_wrapping(char *string, u08 base_pos_x, u08 base_pos_y) {
    u08 pos_x, pos_y;
    // prints left to right
    // will wrap around a scrolling text that exceeds the screen size
    clear_screen();
    num_chars = strlen(string);
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
        set_cursor(pos_x, pos_y);
        print_string(string[i]);
    }
    return;
}

int main(void) {
    u08 pos_x = 0; //  position x value
    u08 pos_y = 0; //  position y value

    char display_text[25];
    char msg[4] = "416";
    u08 position = 0;   // starting index of '416' in display_text
    init();  //initialize board hardware
    print_string_wrapping("Hello", 0, 0);

    while(1) {
        // position = get_msg_position(4);

        // // reset display_text while inserting msg

        // print_string(display_text);
        // _delay_ms(25);
    }

    return 0;
}