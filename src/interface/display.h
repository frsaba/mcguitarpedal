#pragma once
#include <TFT_eSPI.h> 
#include <effects/effect.h>
#include <lvgl.h>
#include "encoder_input.h"
#include <presets/presets.h>
#include <interface/input_events.h>
#include <interface/fonts/symbols_large.h>
#include <interface/fonts/font_fa_icons_16.h>

// #define TFT_RST 34
// #define TFT_DC 9
// #define TFT_CS 36
// #define MOSI 11
// #define MISO 12
// #define SCLK 13
// #define T_CS 33

extern TFT_eSPI  tft;
class Effect;

extern preset_bank_t preset_bank;

LV_FONT_DECLARE(symbols_large)
LV_FONT_DECLARE(font_fa_icons_16)


void init_display();
void create_effect_lists(Effect *effects_chain[], size_t chain_length);
void update_value_label(lv_obj_t* obj, float new_value, char* unit_str);
void update_arc(lv_obj_t* obj, float value);
FLASHMEM lv_obj_t * create_arc(lv_obj_t* parent, float value, int32_t size);
void apply_param_values_to_knobs();