#pragma once
#include <lvgl.h>
#include <effects/effect.h>

extern lv_group_t * effects_group;
extern lv_group_t * params_group;
extern lv_group_t * presets_group;

extern lv_obj_t * list1;
extern lv_obj_t * presets_list;

extern lv_obj_t *label;
extern preset_bank_t preset_bank;

extern size_t chain_length;
extern lv_obj_t** params_lists;

void param_selected_event(lv_event_t * e);
void param_encoder_turned(int enc_diff);
void bypass_event(lv_event_t * e);
void effect_focused_event(lv_event_t * e);
void preset_pressed(lv_event_t * e);
void preset_long_press(lv_event_t * e);