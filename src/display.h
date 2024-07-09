#pragma once
#include <TFT_eSPI.h> 
#include <effects/effect.h>
#include <lvgl.h>
#include "encoder_input.h"

// #define TFT_RST 34
// #define TFT_DC 9
// #define TFT_CS 36
// #define MOSI 11
// #define MISO 12
// #define SCLK 13
// #define T_CS 33

extern TFT_eSPI  tft;
class Effect;

void displayText(const String &s);
void init_display();
void create_effect_lists(Effect *effects_chain[], size_t chain_length);