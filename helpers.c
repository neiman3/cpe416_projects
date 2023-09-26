#include "./library/globals.h"
#include <util/delay.h>
#include <avr/io.h>


void delay_us(uint16_t delay_length) {
    // delay with parameter in mocroseconds
    for (uint16_t delay_counter=0; delay_counter<delay_length; delay_counter++) {
        _delay_us(1);
    }
}

void delay_ms(uint16_t delay_length) {
    for (uint16_t delay_counter=0; delay_counter<delay_length; delay_counter++) {
        _delay_ms(1);
    }
}

void blocking_pwm(uint8_t duty_cycle, uint16_t period_ms, uint8_t pin) {
    // on time = period * duty cycle / 255
    // off time = period - on time
    // delay on_time
    for(int i=0;i<10;i++){
        uint16_t on_time = period_ms * duty_cycle / 255;
        led_on(pin);
        delay_us(on_time * 100);
        led_off(pin);
        delay_us((period_ms - on_time) * 100);
        return;
    }
    
}