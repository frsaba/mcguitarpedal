#include <encoder_input.h>


lv_indev_t* preset_selector;
lv_indev_t* param_selector;
lv_indev_t* value_selector;

typedef struct {
	Encoder* encoder;
	OneButton* button;
	int prev_encoder_value;
	void(*encoder_scroll_cb)(int); //TODO: ability to register multiple event functions
} input_device_data;


void set_scroll_callback(lv_indev_t* indev, void(*callback)(int)){
	input_device_data* idd = (input_device_data*)lv_indev_get_user_data(indev);
	idd->encoder_scroll_cb = callback;
}

void encoder_read(lv_indev_t * indev, lv_indev_data_t*data){
	input_device_data* idd = (input_device_data*)lv_indev_get_user_data(indev);
	//Divide by 4 as there are 4 pulses sent for every encoder step
	int encoder_value = (idd->encoder->read() + 2) / 4;
	
	data->enc_diff = encoder_value - idd->prev_encoder_value;

	// Call encoder turned even if it is set. 
	// Would replace this with lv_indev_add_event but that doesn't seem to work?
	if(idd->encoder_scroll_cb && abs(data->enc_diff)){
		idd->encoder_scroll_cb(data->enc_diff);
	}
	idd->prev_encoder_value = encoder_value;

	data->state = idd->button->debouncedValue() ? LV_INDEV_STATE_RELEASED : LV_INDEV_STATE_PRESSED;
}

void setup_input_devices(){
	preset_selector = lv_indev_create();
	lv_indev_set_user_data(preset_selector, new input_device_data{&encoder_1, &button_1});
	lv_indev_set_type(preset_selector, LV_INDEV_TYPE_ENCODER);
	lv_indev_set_read_cb(preset_selector, encoder_read);

	param_selector = lv_indev_create();
	lv_indev_set_user_data(param_selector, new input_device_data{&encoder_2, &button_2});
	lv_indev_set_type(param_selector, LV_INDEV_TYPE_ENCODER);
	lv_indev_set_read_cb(param_selector, encoder_read);

	value_selector = lv_indev_create();
	lv_indev_set_user_data(value_selector, new input_device_data{&encoder_3, &button_3});
	lv_indev_set_type(value_selector, LV_INDEV_TYPE_ENCODER);
	lv_indev_set_read_cb(value_selector, encoder_read);
}
