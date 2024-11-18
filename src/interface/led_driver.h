#pragma once
#include <IntervalTimer.h>
#define LED_SHIFT_CLOCK 22
#define LED_STORE_CLOCK 13
#define LED_DATA 14
#define LED_PWM 2

#define NUM_LEDS 8
// #define LED_SHIFT_FREQ 100000
// #define LED_SHIFT_PERIOD 1.0 / LED_SHIFT_FREQ

extern IntervalTimer led_timer;

void setup_leds();
void write_to_shift_register(uint8_t data);