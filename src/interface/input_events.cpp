#include <interface/input_events.h>

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

	lv_obj_t* arc = lv_obj_get_child_by_type(selected_param_obj, 0, &lv_arc_class);

	//Update arc
	if (arc) {
		lv_arc_set_value(arc, param->get_as_percentage());
	}
	else{
		LV_LOG_USER("Arc object not found");
	}

	//Update value label
	lv_obj_t* label = lv_obj_get_child_by_type(selected_param_obj, -1, &lv_label_class); // oldest label is the param name, youngest should be the value
	if (label) {
		// TODO: don't put a space before '%' in the unit string
		 lv_label_set_text_fmt(label, "%.1f%s", new_value, param->unit.begin());
	}
	else{
		LV_LOG_USER("Value label object not found");
	}

	//if the param is dry/wet, update the arc next to the effect name as well
	if(&effect->params[0] == param){
		LV_LOG_USER("Changing dry/wet");
		lv_obj_t* arc = lv_obj_get_child_by_type(selected_effect_obj, 0, &lv_arc_class);

		//Update arc
		if (arc) {
			lv_arc_set_value(arc, param->get_as_percentage());
		}
		else{
			LV_LOG_USER("Dry/wet arc object not found");
		}
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
	lv_obj_t* arc = lv_obj_get_child_by_type(target, 0, &lv_arc_class);// lv_obj_get_child(target, 1); // First child is the label, second should be the arc

	if (lv_obj_check_type(arc, &lv_arc_class)) {
		
		lv_arc_set_value(arc, new_value);
		// lv_obj_invalidate(arc); // Force redraw
	}
	else{
		LV_LOG_USER("Arc object not found");
	}


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

void preset_pressed(lv_event_t * e)
{
    // lv_obj_t * target = (lv_obj_t *)lv_event_get_target(e);
	// Effect* effect = (Effect *)target->user_data;
	// int effect_index = lv_obj_get_index(target) - 1;

	LV_LOG_USER("Preset button pressed");
	// preset_bank_t loaded_bank;
	load_presets(&preset_bank);
	LV_LOG_USER("Loaded preset bank");
	save_presets(preset_bank, true);

}

void preset_long_press(lv_event_t * e)
{
    // lv_obj_t * target = (lv_obj_t *)lv_event_get_target(e);
	// Effect* effect = (Effect *)target->user_data;	
	// int effect_index = lv_obj_get_index(target) - 1;

	save_presets(preset_bank);

	LV_LOG_USER("Preset button long pressed");
}