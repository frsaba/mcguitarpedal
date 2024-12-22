#include "settings.h"

lv_obj_t* settings_list;

Param settings_params[NUM_SETTINGS] = 
{
	//TODO: log curve?
	//TODO: turn on all leds while editing this param
	Param("LED brightness", roundf(LED_DEFAULT_BRIGHTNESS / 255.0 * 100 / BRIGHTNESS_STEPS ) * BRIGHTNESS_STEPS, 5,100,BRIGHTNESS_STEPS, [](float val){analogWrite(LED_PWM, 255 - val / 100.0 * 255);}, LV_SYMBOL_UP, "%"),
	//FIXME: backlight pwm pin declaration
	Param("Backlight", 100, 15, 100, BRIGHTNESS_STEPS, [](float val){analogWrite(37, val / 100 * MAX_BRIGHTNESS);}, LV_SYMBOL_IMAGE, "%")
};

void settings_setup()
{


	settings_list = lv_list_create(lv_screen_active());
	lv_obj_set_size(settings_list, lv_pct(100), lv_pct(75));
    lv_obj_align(settings_list, LV_ALIGN_BOTTOM_MID, 0, -3);
	lv_obj_set_style_pad_all(settings_list, 30, LV_STATE_ANY);
	lv_list_add_text(settings_list, "Settings");

	for (size_t i = 0; i < NUM_SETTINGS; i++)
	{
		auto param_button = create_param_row(&settings_params[i], settings_list);
		lv_group_add_obj(params_group, param_button);
		lv_obj_set_user_data(param_button, &settings_params[i]);

	}
	

	settings_hide();
}

void settings_hide()
{
	lv_obj_add_flag(settings_list, LV_OBJ_FLAG_HIDDEN);
}

void settings_show()
{
	lv_obj_remove_flag(settings_list, LV_OBJ_FLAG_HIDDEN);
}


