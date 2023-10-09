/**
 * 
 *
 *   Name:  Alex Neiman and Beck Dehlsen
 *   CPE 416
 *   Lab 2 part 4
 *
 *   Description: Line following robot
 *
 **/


#include "./library/globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#define VWL 145 // white threshold for left
#define VWR 138 // white threshold for right
#define VBL 205 // black threshold for left
#define VBR 205 // black threshold for right

#define TIMESTEP 100
#define SERVO_CAL 40


void motor(uint8_t num, int8_t speed) {
    int32_t sp = ((int32_t) speed * SERVO_CAL / 200) + 127;
    if (num == 1) { // selected first wheel
        // reverse right wheel
        set_servo(num, 255 - sp - 1);
    } else {
        set_servo(num, sp);
    }
    return;
}

int main(void) {
   typedef enum {MODE_3A, MODE_3B} fsm_mode;
   u08 sensor_pins[2] = {3,4}; // Analog pins that correspond to sensors to read
   u08 sensor_value[2]; // sensor values array

   init();  //initialize board hardware
   init_servo();
   init_adc();
   init_lcd();
   motor(0,0);
   motor(1,0);

    while(1) {
        // read ADC and do transform
        for(u08 i=0;i<sizeof(sensor_pins);i++) {
            u16 result;
            // Read ADC value
            result = analog(sensor_pins[i]);
            // do some transform or data processing
            sensor_value[i] = result;
        }

        float theta;
        lcd_cursor(0,0);
        print_num((sensor_value[0] - VWL*0));print_string("   ");
        lcd_cursor(0,1);
        print_num((sensor_value[1] - VWR*0));print_string("   ");

        // calculate 2d mapping transformation of angular difference of sensor readings
        theta = atanf((float) (sensor_value[0] - VWL) / (float) (sensor_value[1] - VWR)) * 180 / 3.14;
        u16 theta_deg = (u16) theta;
        lcd_cursor(4,0);
        print_num(theta_deg);print_string("   ");

        // calculate vstate vector magnitude to determine black, white, etc
        float vstate;
        vstate = (float) sqrt((double) square(VBL - sensor_value[0]) + (double) square(VBR - sensor_value[1]));
        lcd_cursor(4,1);
        print_num((u16) (vstate));print_string("   ");


        if(1) {     // proportional control mode
            
        } else if (1) {    // outside threshold for proportional control - 1 value too low
            if(sensor_value[0]) {
                motor(0, 0);
                motor(1, 0);
            }
            else {

            }
        } else {       // neither sensor reading the tape
            
        }

        _delay_ms(TIMESTEP);
    }



   return 0;
}

