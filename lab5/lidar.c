#include "lidar.h"
#include "../library/globals.h"

#define SCAN_ANGLESTEP 2
#define SCAN_TIMESTEP 10
#define SCAN_ANGLE_WIDTH 20

/*  Move scanner from +- angles, finding coordinates of any objects in range.\
    Returns the angle of the closest target
*/
int16_t full_scan(int8_t scan_direction, uint8_t full_sweep, int16_t init_angle) {
    int16_t sweep_start, sweep_stop;

    if (full_sweep) {
        sweep_start = SWEEP_ANGLE_MIN;
        sweep_stop = SWEEP_ANGLE_MAX;
    } else {
        sweep_start = init_angle - SCAN_ANGLE_WIDTH;
        sweep_stop = init_angle + SCAN_ANGLE_WIDTH;
    }

    int16_t angle = 0;
    uint8_t scan_result;
    uint8_t magnitude = 0;

    if (scan_direction > 0) {
        // sweep min to max
        point_servo(SWEEP_ANGLE_MIN);
        wait_for_servo(angle, SWEEP_ANGLE_MIN);
        for (int16_t i=sweep_start;i<=sweep_stop;i+=SCAN_ANGLESTEP){
            point_servo(i);
            scan_result = analog(PIN_SENSOR_DIST);
            // lcd_cursor(0,0);print_string("Svo:");print_signed(i);print_string("  ");
            // lcd_cursor(0,1);print_string("Dst:");print_num(scan_result);print_string("  ");
            if (scan_result > magnitude) {
                magnitude = scan_result;
                angle = i;
            }
            _delay_ms(SCAN_TIMESTEP);
        }
    } else {
        point_servo(SWEEP_ANGLE_MAX);
        wait_for_servo(angle, SWEEP_ANGLE_MAX);
        for (int16_t i=sweep_stop;i>=sweep_start;i-=SCAN_ANGLESTEP){
            point_servo(i);
            scan_result = analog(PIN_SENSOR_DIST);
            // lcd_cursor(0,0);print_string("Svo:");print_signed(i);print_string("  ");
            // lcd_cursor(0,1);print_string("Dst:");print_num(scan_result);print_string("  ");
            if (scan_result > magnitude) {
                magnitude = scan_result;
                angle = i;
            }
            _delay_ms(SCAN_TIMESTEP);
        }
    }
    return angle - magnitude * 27 / 120;
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
