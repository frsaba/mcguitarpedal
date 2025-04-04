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
extern lv_obj_t* main_container;
extern lv_obj_t * effects_list;
extern lv_obj_t * presets_list;

extern lv_obj_t *log_label;  // Label for log messages

class Effect;

extern preset_bank_t preset_bank;

LV_FONT_DECLARE(symbols_large)
LV_FONT_DECLARE(font_fa_icons_16)


void init_display();
FLASHMEM void create_effect_lists(Effect *effects_chain[], size_t chain_length);
void update_value_label(lv_obj_t* obj, const Param& param);
void update_arc(lv_obj_t* obj, float value);
FLASHMEM lv_obj_t * create_arc(lv_obj_t* parent, float value, int32_t size);
void sync_ui_to_effect_params();
FLASHMEM lv_obj_t* create_param_row(Param* param, lv_obj_t* parent);

void hide_main_screen();
void show_main_screen();