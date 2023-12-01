#include "lidar.h"


extern volatile uint16_t right_encoder;


/*  Move scanner from +- angles, finding coordinates of any objects in range.\
    Returns the angle of the closest target
*/
int16_t scan(int8_t scan_direction, uint8_t threshold_value) {
    int16_t sweep_start, sweep_stop;
    int16_t angle = 0;
    uint8_t scan_result;
    uint8_t magnitude = 0;
    u08 sensor_pins[2] = {3,4}; // Analog pins that correspond to sensors to read
    u08 sensor_value[2]; // sensor values array

    if (scan_direction > 0) {
        // scan clockwise
        sweep_start = SWEEP_ANGLE_MIN;
        sweep_stop = SWEEP_ANGLE_MAX;
    } else {
        // scan counterclockwise
        sweep_start = SWEEP_ANGLE_MAX;
        sweep_stop = SWEEP_ANGLE_MIN;

    }


    // TODO: Include breakout for the line scanner in the loop (return)
    /* for(u08 i=0;i<2;i++) {
            // Read ADC value
            sensor_value[i] = analog(sensor_pins[i]);;
        }
    } while (sensor_value[0] < LINE_THRESHOLD || sensor_value[1] < LINE_THRESHOLD); Code from the sensor reading part */
    // sweep min to max

    point_servo(sweep_start);//     -128            +128            -5
                    for (int16_t i=sweep_start;i<=sweep_stop;i+=SCAN_ANGLESTEP*scan_direction){
        point_servo(i);

        // check sensor readings
        for(u08 i=0;i<2;i++) {
            // Read ADC value
            sensor_value[i] = analog(sensor_pins[i]);;
        }

        if (sensor_value[0] > threshold_value || sensor_value[1] > threshold_value) {
            return BOUNDARY_WARNING;  // return code for line boundary warning
        }

        _delay_ms(SCAN_TIMESTEP);
        scan_result = analog(PIN_SENSOR_DIST);
        if (scan_result > magnitude) {
            magnitude = scan_result;
            angle = i;
        }
    }

    // lcd_cursor(0,1);print_signed(magnitude);
    return angle;
}


/*  Full scan involves a compounding sweep
    If the sweep angle is estimated to be 90º, then the robot will turn an additional ±60º for a full 270º sweep


*/
int16_t full_scan() {

    int16_t angle = 0;
    uint8_t magnitude = 0;
    int16_t servo_position;
    uint8_t scan_result;

    // robot rotates 180º =  60 encoder ticks

    // reset encoder
    // point lidar all the way to the left
    // rotate left by 25 encoder ticks
    right_encoder = 0;
    point_servo(SWEEP_ANGLE_MIN);
    wait_for_servo(SWEEP_ANGLE_MAX,SWEEP_ANGLE_MIN);
    while(right_encoder < TICKS_TO_90_DEGREES_CONV) {
        motor(0,-FWD_SPEED*2);
        motor(1,FWD_SPEED*2);
        _delay_ms(10);
    }
    // reset encoder
    stop_motor();
    right_encoder = 0;
    clear_screen();
    while (right_encoder < TICKS_TO_90_DEGREES_CONV*2) {
        servo_position = (int16_t)(right_encoder-(int16_t) TICKS_TO_90_DEGREES_CONV) * (int16_t) 127 / (int16_t) TICKS_TO_90_DEGREES_CONV;
        point_servo(servo_position);
        motor(0,FWD_SPEED * 2);
        motor(1,-FWD_SPEED * 2);
        scan_result = analog(PIN_SENSOR_DIST);
         // lcd_cursor(0,0);print_string("Svo:");print_signed(i);print_string("  ");
         // lcd_cursor(0,1);print_string("Dst:");print_num(scan_result);print_string("  ");
        if (scan_result > magnitude) {
            magnitude = scan_result;
            angle = compound_angle_calculation(servo_position, right_encoder);
            clear_screen();
            lcd_cursor(0,1);print_signed(servo_position);print_string(" deg   ");
            lcd_cursor(0,0);print_string("Seek can");
        }
        _delay_ms(5);
    }
    stop_motor();
    return angle;
}

void point_servo(int16_t angle) {
    if (angle > 128) {
        angle = 128;
    }
    if (angle < -128) {
        angle = -128;
    }
    set_servo(2, 128 - angle);
}

void print_signed(int16_t value){
    if (value > 0) {
        print_num(value);
    }
    else {
        print_string("-");
        print_num(-value);
    }
}

void wait_for_servo(int16_t current, int16_t target) {
    // Delay estimate for servo to reach steady state
    // takes about 120 ms for full 255 steps
    for (int16_t i=0; i<abs(current-target) * 120 / 255;i++){
        _delay_ms(1);
    }
}

int16_t compound_angle_calculation(int16_t servo_position, int16_t encoder_value) {
    int16_t servo_angle, robot_angle;
    servo_angle = (int16_t) 45 * servo_position / (int16_t) 127;
    robot_angle = (int16_t) 90 * (int16_t) ( (int16_t) right_encoder - (int16_t) TICKS_TO_90_DEGREES_CONV ) / (int16_t)TICKS_TO_90_DEGREES_CONV;
    return robot_angle + servo_angle;
}