#include <encoder_input.h>


lv_indev_t* effect_selector;
lv_indev_t* param_selector;
lv_indev_t* value_selector;

int selector_prev;

typedef struct {
	Encoder* encoder;
	OneButton* button;
	int prev_encoder_value;
} input_device_data;


void encoder_read(lv_indev_t * indev, lv_indev_data_t*data){
	input_device_data* idd = (input_device_data*)lv_indev_get_user_data(indev);
	int encoder_value = idd->encoder->read() / 4;
	
	data->enc_diff = encoder_value - idd->prev_encoder_value;
	idd->prev_encoder_value = encoder_value;

	data->state = idd->button->debouncedValue() ? LV_INDEV_STATE_RELEASED : LV_INDEV_STATE_PRESSED;
}

void setup_input_devices(){
	effect_selector = lv_indev_create();
	lv_indev_set_user_data(effect_selector, new input_device_data{&encoder_1, &button_1});
	lv_indev_set_type(effect_selector, LV_INDEV_TYPE_ENCODER);
	lv_indev_set_read_cb(effect_selector, encoder_read);

	param_selector = lv_indev_create();
	lv_indev_set_user_data(param_selector, new input_device_data{&encoder_2, &button_2});
	lv_indev_set_type(param_selector, LV_INDEV_TYPE_ENCODER);
	lv_indev_set_read_cb(param_selector, encoder_read);

	value_selector = lv_indev_create();
	lv_indev_set_user_data(value_selector, new input_device_data{&encoder_3, &button_3});
	lv_indev_set_type(value_selector, LV_INDEV_TYPE_ENCODER);
	lv_indev_set_read_cb(value_selector, encoder_read);
}
