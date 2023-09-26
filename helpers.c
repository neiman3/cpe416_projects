#include "./library/globals.h"
#include <util/delay.h>
#include <avr/io.h>

const u16 DELAY_INC_MS = 1;
const u16 DELAY_INC_US = 1;

//incremental delay in microseconds
void inc_delay_us(uint16_t delay_length_us) {
    for (uint16_t delay_counter=0; delay_counter<delay_length_us; delay_counter+=DELAY_INC_US) {
        _delay_us(DELAY_INC_US);
    }
}

//incremental delay in milliseconds
void inc_delay_ms(uint16_t delay_length_ms) {
    for (uint16_t delay_counter=0; delay_counter<delay_length_ms; delay_counter+=DELAY_INC_MS) {
        _delay_ms(DELAY_INC_MS);
    }
}

void blocking_pwm(uint8_t duty_cycle, uint16_t period_us, uint8_t pin) {
    // on time = period * duty cycle / 255
    // off time = period - on time
    // delay on_time
    uint16_t on_time = period_us * duty_cycle / 255;
    led_on(pin);
    inc_delay_us(on_time);
    led_off(pin);
    inc_delay_us(period_us - on_time);
    return;
}

void my_blocking_pwm(u08 duty_cycle, u16 period_us, u08 pin) {
    for(u16 delay_ms=10; delay_ms>=1; delay_ms--) {
        led_on(pin);
        _delay_ms(delay_ms);
        led_off(pin);
        _delay_ms(delay_ms);
    }
}