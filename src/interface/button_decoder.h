#pragma once

#include <Arduino.h>

#define BUTTON_MTX_A 41
#define BUTTON_MTX_B 40
#define BUTTON_MTX_C 24

#define BUTTON_MTX_GS 9

#define BUTTON_TUNER 7
#define BUTTON_BANK 6

void setup_decoder();
void get_pressed_button();