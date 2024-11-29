#pragma once

#include <Arduino.h>
#include <OneButton.h>
#include <lvgl.h>

#define BUTTON_MTX_A 41
#define BUTTON_MTX_B 40
#define BUTTON_MTX_C 24

#define BUTTON_MTX_GS 9

#define BUTTON_TUNER 7
#define BUTTON_BANK 6

#define LONG_PRESS_TIME 600

extern void (*short_press_release_function_map[8])();
extern void (*long_press_start_function_map[8])();

extern uint8_t current_button_code;
extern uint8_t last_button_code;
extern bool any_button_pressed;
extern bool prev_any_button_pressed;
extern uint32_t last_press_start_time;
extern bool long_press_ran;

void setup_decoder();
void decoder_tick();
void decoder_attach_click(uint8_t button_index, callbackFunction);
void decoder_attach_long_press(uint8_t button_index, callbackFunction);