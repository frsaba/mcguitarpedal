#include <encoder_input.h>


lv_indev_t* encoder1;
lv_indev_t* encoder2;

int selector_prev;


void encoder_read(lv_indev_t * indev, lv_indev_data_t*data){
	int encoder_value = param_selector.read() / 4;
	data->enc_diff = encoder_value - selector_prev;
	selector_prev =  encoder_value;

	data->state = button2.debouncedValue() ? LV_INDEV_STATE_RELEASED : LV_INDEV_STATE_PRESSED;
}

void setup_input_devices(){
	encoder1 = lv_indev_create();
	lv_indev_set_type(encoder1, LV_INDEV_TYPE_ENCODER);
	lv_indev_set_read_cb(encoder1, encoder_read);
}
