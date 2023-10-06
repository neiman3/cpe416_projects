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
void motor(uint8_t num, int8_t speed) {
    int32_t sp = ((int32_t) speed * SERVO_CAL / 200) + 127;
    if (num == 1) { // selected first wheel
        // reverse right wheel
        lcd_cursor(0,1);print_num(255 - sp - 1);print_string("   ");
        set_servo(num, 255 - sp - 1);
    } else {
        lcd_cursor(0,0);print_num(sp);print_string("   ");
        set_servo(num, sp);
    }
    return;
}


int main(void) {

    init();  //initialize board hardware
    
    while (1) {
        for (int8_t i=0; i<=AMPLITUDE; i++) {
            motor(0,i);
            motor(1,i);
            lcd_cursor(0,0);
            clear_screen();
            print_string("s1:");
            print_num(i);
            _delay_ms(TIMESTEP);
        }
        for (int8_t i=AMPLITUDE; i>=0; i--) {
            motor(0,i);
            motor(1,i);
            clear_screen();
            print_string("s2:");
            print_num(i);
            _delay_ms(TIMESTEP);
        }
        for (int8_t i=0; i>=-1*AMPLITUDE; i--) {
            motor(0,i);
            motor(1,i);
            clear_screen();
            print_string("s3:");
            print_num(i);
            _delay_ms(TIMESTEP);
        }
        for (int8_t i=-1*AMPLITUDE; i<=0; i++) {
            motor(0,i);
            motor(1,i);
            clear_screen();
            print_string("s4:");
            print_num(i);
            _delay_ms(TIMESTEP);
        }
    }

    return 0;
}

