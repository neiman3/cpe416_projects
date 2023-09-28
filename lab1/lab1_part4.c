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

#include "../library/globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>


// calculate starting index of msg using get_accel funcs
u08 get_msg_position(u08 msg_size) {
    u08 pos;

    return pos;
}

int main(void) {
    char display_text[25];
    char msg[4] = "416";
    u08 position = 0;   // starting index of '416' in display_text
    init();  //initialize board hardware

    while(1) {
        position = get_msg_position(4);

        // reset display_text while inserting msg

        print_string(display_text);
        _delay_ms(25);
    }

    return 0;
}