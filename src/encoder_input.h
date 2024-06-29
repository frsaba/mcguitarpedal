#pragma once
#include <lvgl.h>
#include <OneButton.h>
#include <Encoder.h>

extern lv_indev_t* encoder1;
extern lv_indev_t* encoder2;

extern OneButton button2;
extern Encoder param_selector;
extern int selector_prev;

void encoder_read(lv_indev_t * indev, lv_indev_data_t*data);
void setup_input_devices();

