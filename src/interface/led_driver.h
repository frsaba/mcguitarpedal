#pragma once
#include <IntervalTimer.h>
#include "statusbar.h"
#include <Arduino.h>

#define LED_SHIFT_CLOCK 22
#define LED_STORE_CLOCK 13
#define LED_DATA 14
#define LED_PWM 2
#define LED_DEFAULT_BRIGHTNESS 50

#define NUM_LEDS 8
#define LED_TUNER 0x1
#define LED_BYPASS 0x2
#define LED_STATUS 0x4
#define LED_BANK 0x8
#define LED_PRESET_1 0x80
#define LED_PRESET_2 0x40
#define LED_PRESET_3 0x20
#define LED_PRESET_4 0x10

// #define LED_SHIFT_FREQ 100000
// #define LED_SHIFT_PERIOD 1.0 / LED_SHIFT_FREQ

// extern IntervalTimer led_timer;

extern uint8_t led_state;
extern uint8_t preset_leds[];

void setup_leds();
void write_to_shift_register(uint8_t data);
void led_toggle(uint8_t led);
void led_set(uint8_t led, bool);
void led_set_preset(uint8_t preset_index);
void led_set_tuner(uint8_t mask);