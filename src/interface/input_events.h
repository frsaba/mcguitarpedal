#pragma once
#include <lvgl.h>
#include <effects/effect.h>
#include "button_decoder.h"
#include <encoder_input.h>
#include "statusbar.h"
#include "led_driver.h"

extern bool tuner_mode;

extern Effect *effects_chain[];

extern lv_group_t * effects_group;
extern lv_group_t * params_group;
extern lv_group_t * presets_group;

extern lv_obj_t * effects_list;
extern lv_obj_t * presets_list;

extern lv_obj_t *label;
extern preset_bank_t preset_bank;

extern size_t chain_length;
extern lv_obj_t** params_lists;

void preset_buttons_read();
void setup_input_devices();
void setup_button_events();
void previous_preset();
void next_preset();
void toggle_tuner_mode();
void param_selected_event(lv_event_t * e);
void param_encoder_turned(int enc_diff);
void bypass_event(lv_event_t * e);
void effect_focused_event(lv_event_t * e);
void load_preset(size_t slot);
void save_preset(size_t slot);