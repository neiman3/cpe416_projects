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


// Tunings and threhsold
#define VWL 170 // white threshold for left
#define VWR 170 // white threshold for right
#define VBL 212 // black threshold for left
#define VBR 212 // black threshold for right
#define VSTATE_W_W 90 // white on white Vstate threshold
#define VSTATE_B_B 20 // black on black threshold

#define GAIN_KP_NUM 20
#define GAIN_KP_DEN 10

#define GAIN_KI_NUM 10
#define GAIN_KI_DEN 10

#define GAIN_KD_NUM 10
#define GAIN_KD_DEN 10

#define SKIP_CAL 0 // make 1 to skip calibration
#define DEBUG_MODE 1 // make motor turn off when button held
#define TIMESTEP 100
#define SERVO_CAL 40
#define FWD_SPEED 30 // Default forward speed
#define THETA_FWD 45 // straight angle (zero/forward)

float calculate_vstate_vector(u08 vbl_set, u08 sensor_0, u08 vbr_set, u08 sensor_1) {
    return (float) sqrt((double) square(vbl_set - sensor_0) + (double) square(vbr_set - sensor_1));
}

float calculate_theta(u08 sensor_0, u08 vwl_set, u08 sensor_1, u08 vwr_set) {
    return atanf((float) (sensor_0 - vwl_set) / (float) (sensor_1 - vwr_set)) * 180 / 3.14;
}


int16_t min(int16_t arg1, int16_t arg2) {
    if (arg1 < arg2){
        return arg1;
    }
    return arg2;
}

int16_t max(int16_t arg1, int16_t arg2) {
    if (arg1 > arg2){
        return arg1;
    }
    return arg2;
}

int16_t bound(int16_t arg, int16_t minimum, int16_t maximum) {
    
    return max(minimum, min(maximum, arg));
}

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


void motor_dir(int16_t angle) {
    // takes  a signed 8 bit "angle" +- 200 deg
    // robot is turning with two radii so it's not really an angle...
    // debugging mode = if button pressed, disable motor
    angle = bound(angle, -100, 100);
    
    int16_t power_l = FWD_SPEED;
    int16_t power_r = FWD_SPEED;
    if (angle < 0) {
        angle = angle * -1;
        // turn left, cut power to left motor and boost power to right motor
        power_l -= (angle * FWD_SPEED / 100);
        power_r += (angle * FWD_SPEED / 100);
    }
    else if (angle > 0) {
        // turn right, cut power to right motor and boost power to left motor
        power_l += (angle * FWD_SPEED / 100);
        power_r -= (angle * FWD_SPEED / 100);
    }
    power_l = bound(power_l, 0, 100);
    power_r = bound(power_r, 0, 100);

    if (DEBUG_MODE) {
        if (get_btn()) {
            power_l = 0; power_r = 0;
        }
    }

    motor(0, power_l);
    motor(1, power_r);
}
void wait_for_button() {
    _delay_ms(10);
    while(get_btn()) {
        // wait for button to become available / unpressed
        _delay_ms(10);
    }
    while(!get_btn()) {
        // wait for user to press button
        _delay_ms(10);
    }
    while(get_btn()) {
        // while user is holding button
        _delay_ms(10);
    }
}

int main(void) {
    u08 sensor_pins[2] = {3,4}; // Analog pins that correspond to sensors to read
    u08 sensor_value[2]; // sensor values array
    u08 side_last_found = 0; // which sensor read tape most recently - for black-white case

    u08 VWL_set = VWL;
    u08 VWR_set = VWR;
    u08 VBL_set = VBL;
    u08 VBR_set = VBR;
    u08 VSTATE_B_B_set = VSTATE_B_B;
    u08 VSTATE_W_W_set = VSTATE_W_W;

    init();  //initialize board hardware
    init_servo();
    init_adc();
    init_lcd();
    motor(0,0);
    motor(1,0);

    // calibration step
    u08 skip_cal = SKIP_CAL;
    clear_screen();
    lcd_cursor(0,0);
    print_string("CPE416");
    lcd_cursor(0,1);
    print_string("linebot");
    u16 timer =  0;
    for(u16 i=0; i<2000;i++) {
        if (get_btn()){
            timer++;
        } else {
            timer = 0;
        }
        if (timer > 750) {
            // user held the button- this will skip the clalibration
            skip_cal = 1;
            break;
        }
        _delay_ms(1);
    }

    if (skip_cal) {
        clear_screen(); lcd_cursor(0,0); print_string("Cal");lcd_cursor(0,1);print_string("skipped");
        _delay_ms(750);
    } else {

        // CALIBRATION

        clear_screen();
        lcd_cursor(0,1);
        print_string("Cal Blk.");
        lcd_cursor(0,0);
        print_string("Press to");
        wait_for_button();

        // read sensors
        for(u08 i=0;i<sizeof(sensor_pins);i++) {
            u16 result;
            // Read ADC value
            result = analog(sensor_pins[i]);
            // do some transform or data processing
            sensor_value[i] = result;
        }

        VBL_set = min(sensor_value[0] + 5,255);
        VBR_set = min(sensor_value[1] + 5,255);

        clear_screen();
        lcd_cursor(0,1);
        print_string("Cal Wht.");
        lcd_cursor(0,0);
        print_string("Press to");
        wait_for_button();

        // read sensors
        for(u08 i=0;i<sizeof(sensor_pins);i++) {
            u16 result;
            // Read ADC value
            result = analog(sensor_pins[i]);
            // do some transform or data processing
            sensor_value[i] = result;
        }

        VWL_set = max(sensor_value[0] - 5,0);
        VWR_set = max(sensor_value[1] - 5,0);

        

        // calculate Vstate calibration states using white values
        VSTATE_W_W_set = max((calculate_vstate_vector(VBL_set, sensor_value[0], VBR_set, sensor_value[1]) - 5),0);

    }

    // display cal values
    clear_screen(); lcd_cursor(0,0); print_string("Cal");lcd_cursor(0,1);print_string("values");
    _delay_ms(750);

    clear_screen(); // Display text label
    lcd_cursor(0,0); print_string("VWL");
    lcd_cursor(0,1); print_string("VWR");
    lcd_cursor(4,0); print_string("VBL");
    lcd_cursor(4,1); print_string("VBR");
    _delay_ms(750);
    clear_screen();
    lcd_cursor(0,0); print_num(VWL_set);
    lcd_cursor(0,1); print_num(VWR_set);
    lcd_cursor(4,0); print_num(VBL_set);
    lcd_cursor(4,1); print_num(VBR_set);
    _delay_ms(2000);
    clear_screen();
    lcd_cursor(0,0); print_string("VsW");
    lcd_cursor(0,1); print_string("VsB");
    lcd_cursor(4,0); print_num(VSTATE_W_W_set);
    lcd_cursor(4,1); print_num(VSTATE_B_B_set);
    _delay_ms(2000);
    clear_screen();
    lcd_cursor(0,0); print_string("Press to");
    lcd_cursor(0,1); print_string("run bot");
    wait_for_button();


    

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
        // bound
        sensor_value[0] = bound(sensor_value[0], VWL_set, 255);
        sensor_value[1] = bound(sensor_value[1], VWR_set, 255);
        
        lcd_cursor(0,0);
        print_num((sensor_value[0]));print_string("   ");
        lcd_cursor(0,1);
        print_num((sensor_value[1]));print_string("   ");


        // calculate 2d mapping transformation of angular difference of sensor reading
        theta = calculate_theta(sensor_value[0], VWL_set, sensor_value[1], VWR_set);
        u16 theta_deg = (u16) theta;
        lcd_cursor(4,0);
        print_num(theta_deg);print_string("   ");

        // calculate vstate vector magnitude to determine black, white, etc
        float vstate;
        vstate = calculate_vstate_vector(VBL_set, sensor_value[0], VBR_set, sensor_value[1]);
        lcd_cursor(4,1);
        print_num((u16) (vstate));print_string("   ");


        if(vstate < VSTATE_B_B) {  
            lcd_cursor(3,0);print_string("B");   
            // black on black or tape crossing
            // go forward blindly
            motor_dir(0);
            _delay_ms(250);
            
        } else if (vstate > VSTATE_W_W) {    // outside threshold for proportional control - 1 value too low (black-white case)
            //        // neither sensor reading the tape (white-white case)
            // spin in the direction of side_last_found
            lcd_cursor(3,0);print_string("W");  
            if(side_last_found == 0) {  // left
                motor_dir(-100);
            } else {                    //right
                motor_dir(100);
            }

        } else {
            // proportional control mode
            lcd_cursor(3,0);print_string("P");  
            motor_dir(((int16_t) (THETA_FWD - theta)) * GAIN_KP_NUM / GAIN_KP_DEN);
            if (theta < THETA_FWD) { // on the right side
                side_last_found = 1;
            } else {
                side_last_found = 0;
            }
        }

        _delay_ms(TIMESTEP);
    }

   return 0;
}

