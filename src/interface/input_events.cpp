#include <interface/input_events.h>

void preset_buttons_read(lv_indev_t * indev, lv_indev_data_t*data)
{
	//Not implemented because decoder button events are used instead
}

void setup_input_devices(){
	preset_selector = lv_indev_create();
	// lv_indev_set_user_data(preset_selector, new input_device_data{&encoder_1, &button_1});
	// lv_indev_set_type(preset_selector, LV_INDEV_TYPE_ENCODER);
	// lv_indev_set_read_cb(preset_selector, preset_buttons_read);

	param_selector = lv_indev_create();
	lv_indev_set_user_data(param_selector, new input_device_data{&encoder_2, &button_2});
	lv_indev_set_type(param_selector, LV_INDEV_TYPE_ENCODER);
	lv_indev_set_read_cb(param_selector, encoder_read);

	value_selector = lv_indev_create();
	lv_indev_set_user_data(value_selector, new input_device_data{&encoder_3, &button_3});
	lv_indev_set_type(value_selector, LV_INDEV_TYPE_ENCODER);
	lv_indev_set_read_cb(value_selector, encoder_read);

	setup_button_events();
}

void setup_button_events()
{
	decoder_attach_click(BUTTON_TUNER, []() { 
		previous_preset();
	});
	
	decoder_attach_long_press(BUTTON_TUNER, []() { 
		Serial.println(">---- Tuner mode"); 
	});

	decoder_attach_click(BUTTON_BANK, next_preset);
}

//Select previous preset and load it
void previous_preset()
{
	lv_group_focus_prev(presets_group);
	lv_obj_t * focused = lv_group_get_focused(presets_group);
    lv_obj_send_event(focused, LV_EVENT_SHORT_CLICKED, NULL);
	Serial.println(">---- Previous preset"); 
}

void next_preset()
{
	lv_group_focus_next(presets_group);
	lv_group_send_data(presets_group, LV_KEY_ENTER);

	lv_obj_t * focused = lv_group_get_focused(presets_group);
    lv_obj_send_event(focused, LV_EVENT_SHORT_CLICKED, NULL);
	Serial.println(">---- Next preset"); 
}

//Called when the param selection button is pressed. Toggles value set mode
void param_selected_event(lv_event_t * e)
{
    lv_obj_t * target = (lv_obj_t *)lv_event_get_target(e);
	Param* param = (Param *)target->user_data;

	if(lv_obj_has_flag(lv_obj_get_parent(target), LV_OBJ_FLAG_HIDDEN)){
		LV_LOG_USER("Trying to set hidden object");
		return;
	}

	LV_LOG_USER("Clicked: %s", param->name.begin());

	// lv_obj_add_state(target, LV_STATE_PRESSED);
	lv_group_set_editing(params_group, !lv_group_get_editing(params_group));

	if(lv_group_get_editing(params_group)){
		lv_obj_add_state(target, LV_STATE_EDITED);
	}else{
		lv_obj_remove_state(target, LV_STATE_EDITED);
	}
}

void param_encoder_turned(int enc_diff)
{
	//If we are not in edit mode, ignore.
	if(lv_group_get_editing(params_group) == false){
		// LV_LOG_USER("Not in edit mode");
		return;
	} 

	LV_LOG_USER("Encoder turned %d", enc_diff); //%s", lv_event_get_user_data(e));

	lv_obj_t* selected_param_obj = lv_group_get_focused(params_group);
	lv_obj_t* selected_effect_obj = lv_group_get_focused(effects_group);
	if(selected_param_obj == NULL)	LV_LOG_USER("Focused param object is null");
	if(selected_effect_obj == NULL)	LV_LOG_USER("Focused effect object is null");

	//make sure the param is not hidden (in another tab)
	//the parent has the hidden flag
	if( lv_obj_has_flag(lv_obj_get_parent(selected_param_obj), LV_OBJ_FLAG_HIDDEN)){
		LV_LOG_USER("Trying to set hidden param");
		return;
	}

	Param* param = (Param *)lv_obj_get_user_data(selected_param_obj);
	Effect* effect = (Effect *)lv_obj_get_user_data(selected_effect_obj);

	float new_value = param->change(enc_diff);
	LV_LOG_USER("Changing param %s - %s : %.1f", effect->name.begin(), param->name.begin(), new_value);


	update_arc(selected_param_obj, param->get_as_percentage());
	update_value_label(selected_param_obj, new_value, param->unit.begin());

	//if the param is dry/wet, update the arc next to the effect name as well
	if(&effect->params[0] == param){
		LV_LOG_USER("Changing dry/wet");
		update_arc(selected_effect_obj, param->get_as_percentage());
	}

}

//Called when effect encoder button is pressed. Toggles the effect bypass
void bypass_event(lv_event_t * e)
{
    lv_obj_t * target = (lv_obj_t *)lv_event_get_target(e);
	Effect* effect = (Effect *)target->user_data;

	// LV_UNUSED(obj);
	float new_value = effect->toggle_bypass();
	LV_LOG_USER("Clicked: %s", effect->name.begin());

	update_arc(target, new_value);
}

// Called when the effect is navigated to; Hides the old params list and shows the new one instead.
void effect_focused_event(lv_event_t * e)
{
    lv_obj_t * target = (lv_obj_t *)lv_event_get_target(e);
	Effect* effect = (Effect *)target->user_data;
	int effect_index = lv_obj_get_index(target) - 1;

	if(effect_index < 0){
		LV_LOG_ERROR("Could not find focused effect index");
		return;	
	}

	LV_LOG_USER("Focused: %s %d", effect->name.begin(), effect_index);

	for (size_t i = 0; i < chain_length; i++)
	{
		if(i == static_cast<size_t>(effect_index)) lv_obj_remove_flag(params_lists[i], LV_OBJ_FLAG_HIDDEN);
		else lv_obj_add_flag(params_lists[i], LV_OBJ_FLAG_HIDDEN);
	}

	//If we are in the middle of editing a parameter, exit edit mode.
	lv_group_set_editing(params_group, false);
	lv_obj_remove_state(*params_group->obj_focus, LV_STATE_PRESSED);
}

//Load preset that was clicked
void preset_pressed(lv_event_t * e)
{
	lv_obj_t * target = (lv_obj_t *)lv_event_get_target(e);
	int preset_index = lv_obj_get_index_by_type(target, &lv_list_button_class);
	if(preset_index < 0){
		LV_LOG_ERROR("Could not find preset index");
		return;
	}
	// LV_LOG_USER("Preset button pressed");

	// preset_bank_t loaded_bank;
	// load_presets(&preset_bank);
	LV_LOG_USER("Loaded preset bank");
	serialize_presets(preset_bank, false);
	LV_LOG_USER("Applying preset...");
	preset_bank.active_preset = preset_index;
	apply_preset_values(preset_bank.presets[preset_bank.active_preset ].effect_values, effects_chain, preset_bank.num_effects);
	apply_param_values_to_knobs();

	displayText("Loaded preset " + String(preset_bank.active_preset));

}

void preset_long_press(lv_event_t * e)
{
	lv_obj_t * target = (lv_obj_t *)lv_event_get_target(e);
    int preset_index = lv_obj_get_index_by_type(target, &lv_list_button_class);
	preset_bank.active_preset = preset_index;
	if(preset_index < 0){
		LV_LOG_ERROR("Could not find preset index");
		return;
	}
	preset_bank.presets[preset_bank.active_preset] = effects_to_preset_data("Preset " + preset_bank.active_preset, effects_chain);
	serialize_presets(preset_bank, true);

	// LV_LOG_USER("Preset button long pressed");
	displayText("Saved preset " + String(preset_bank.active_preset));
}