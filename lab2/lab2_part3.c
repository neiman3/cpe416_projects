/**
 * 
 *
 *   Name:  Alex Neiman and Beck Dehlsen
 *   CPE 416
 *   Lab 2 part 2
 *
 *   Description: Switch between Braitenburg vehicles 2A and 2B
 * 
 *    3A: attraction...     light causes that side to slow down
 *    3B: shy... light on that side causes other side to slow down
 *
 **/


#include "../library/globals.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define AMBIENT_LIGHT_THRESHOLD 120
#define DEFAULT_SPEED 10
#define GAIN 1
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
   u08 sensor_pins[2] = {0,1}; // Analog pins that correspond to sensors to read
   u08 sensor_value[2]; // sensor values array

   fsm_mode mode = MODE_3A;
   init();  //initialize board hardware
   init_servo();
   init_adc();
   init_lcd();
   motor(0,0);
   motor(1,0);

    print_string("Mode 3A");
    while(1) {
        // read ADC and do transform
        for(u08 i=0;i<sizeof(sensor_pins);i++) {
            u16 result;
            // Read ADC value
            result = analog(sensor_pins[i]);

            if (result <= AMBIENT_LIGHT_THRESHOLD) {
                // below threshold
                result = 0;
            } else {
                result = (((u16) result - AMBIENT_LIGHT_THRESHOLD) * 255 / (255 - AMBIENT_LIGHT_THRESHOLD));
            }
            sensor_value[i] = result;
        }

        // switch case for state machine
        if (get_btn()) {

            // user pressed the button

            // increment mode;
            lcd_cursor(0,0);
            if (mode == MODE_3A) {
                print_string("Mode 3B");
                mode = MODE_3B;
            } else {
                print_string("Mode 3A");
                mode = MODE_3A;
            }

            while (get_btn()) {
                _delay_ms(TIMESTEP);
                // wait for button release
            }

        }
        for (int i=0; i<sizeof(sensor_pins); i++){
            u16 result;
            result = DEFAULT_SPEED + (100-DEFAULT_SPEED) - ((u16) sensor_value[i] * GAIN * (100-DEFAULT_SPEED) / 255);
            if (result > 100) {
                result = 100;
            }

            switch (mode)
            {
            case MODE_3A:
                motor(i, result);
                break;
                
            
            case MODE_3B:
                motor(!i, result);
                break;
            
            default:
                break;
            }
            
        }

            

        _delay_ms(TIMESTEP);
    }



   return 0;
}

