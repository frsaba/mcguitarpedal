#pragma once
#include <lvgl.h>
#include <OneButton.h>
#include <Encoder.h>

typedef struct {
	Encoder* encoder;
	OneButton* button;
	int prev_encoder_value;
	void(*encoder_scroll_cb)(int); //TODO: ability to register multiple event functions
} input_device_data;

extern lv_indev_t* preset_selector;
extern lv_indev_t* param_selector;
extern lv_indev_t* value_selector;

// extern OneButton button_1;
// extern Encoder encoder_1;
extern OneButton button_2;
extern Encoder encoder_2;
extern OneButton button_3;
extern Encoder encoder_3;

void set_scroll_callback(lv_indev_t* indev, void(*encoder_scroll_cb)(int));
void encoder_read(lv_indev_t * indev, lv_indev_data_t*data);
void setup_input_devices();

