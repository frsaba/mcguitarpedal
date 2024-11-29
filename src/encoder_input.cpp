#include <encoder_input.h>


lv_indev_t* preset_selector;
lv_indev_t* param_selector;
lv_indev_t* value_selector;


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
