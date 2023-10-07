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

#define TIMESTEP 5
#define AMPLITUDE 100
#define SERVO_CAL 40

// call set_servo to spin the motor: 0 for left, 1 for right
// speed range: -100 to 100 -> 0 to 255, 127 being zero, +-30  being the range before saturation

// servo value = setpoint * 60 / 200 + 127
// Adjust useable range using servo_cal #define
// inversion (reverse) = 255 - servo value - 1 (prevent overflow by subtracting by 256)
// gain



int main(void) {

    init();  //initialize board hardware
    init_adc();
    init_servo();
    init_lcd();
    set_servo(0,127);
    set_servo(1,127);

    u08 pins[] = {3, 4}; // ppins to read
    u08 xcoord[] = {0, 0, 4, 4}; // coords to print them here
    u08 ycoord[] = {0, 1, 0, 1};
    while(1) {
        for (u08 i=0; i<sizeof(pins); i++){
            lcd_cursor(xcoord[i], ycoord[i]);
            print_num(pins[i]);
            print_string(":");
            print_num(analog(pins[i]));print_string("  ");
            
        }
        _delay_ms(250);
    }

    return 0;
}

