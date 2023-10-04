/**
 * 
 *
 *   Name:  Alex Neiman and Beck Dehlsen
 *   CPE 416
 *   Lab 2 part 1
 *
 *   Description: Spin motors back and forth while displaying speed
 *
 **/


#include "../library/globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// call set_servo to spin the motor: 0 for left, 1 for right
// speed range: -100 to 100
void motor(uint8_t num, int8_t speed) {
    set_servo(0, 127);
}

int main(void) {

    init();  //initialize board hardware

    print_string("Speed: 0");
    motor(0, 0);

    return 0;
}

