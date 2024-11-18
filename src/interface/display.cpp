#include <interface/display.h>
#include <interface/input_events.h>

lv_obj_t *label;
preset_bank_t preset_bank;

void displayText(const String &s)
{
	lv_label_set_text( label, s.begin() );
    Serial.println(s);
}

/*Set to your screen resolution and rotation*/
#define TFT_HOR_RES   480
#define TFT_VER_RES   320
#define TFT_ROTATION  LV_DISPLAY_ROTATION_90

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

/*Read the touchpad*/
void my_touchpad_read( lv_indev_t * indev, lv_indev_data_t * data )
{
    /*For example  ("my_..." functions needs to be implemented by you)
    int32_t x, y;
    bool touched = my_get_touch( &x, &y );

    if(!touched) {
        data->state = LV_INDEV_STATE_RELEASED;
    } else {
        data->state = LV_INDEV_STATE_PRESSED;

        data->point.x = x;
        data->point.y = y;
    }
     */
}

/*use Arduinos millis() as tick source*/
static uint32_t my_tick(void)
{
    return millis();
}

lv_group_t * effects_group;
lv_group_t * params_group;
lv_group_t * presets_group;

lv_obj_t * effects_list;
lv_obj_t * presets_list;

size_t chain_length;
lv_obj_t** params_lists;

// This event gets called when the params encoder gets turned. We should react and change the actively edited parameter
typedef struct {
    TFT_eSPI * tft;
} lv_tft_espi_t;

// Overriding the display flush callback, swapping the color byte order 
static void swapped_flush_cb(lv_display_t * disp, const lv_area_t * area, uint8_t * px_map)
{
    lv_tft_espi_t * dsc = (lv_tft_espi_t *)lv_display_get_driver_data(disp);

    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    dsc->tft->startWrite();
    dsc->tft->setAddrWindow(area->x1, area->y1, w, h);
    dsc->tft->pushColors((uint16_t *)px_map, w * h, false);
    dsc->tft->endWrite();

    lv_display_flush_ready(disp);

}

FLASHMEM void init_display()
{
    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.println( LVGL_Arduino );

    lv_init();

    /*Set a tick source so that LVGL will know how much time elapsed. */
    lv_tick_set_cb(my_tick);

    /* register print function for debugging */
// #if LV_USE_LOG != 0
//     lv_log_register_print_cb( my_print );
// #endif

    lv_display_t * disp;
    /*TFT_eSPI can be enabled lv_conf.h to initialize the display in a simple way*/
    disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, sizeof(draw_buf));
	lv_display_set_flush_cb(disp, swapped_flush_cb);
    // lv_display_set_rotation(disp, TFT_ROTATION);
	lv_tft_espi_t * dsc = (lv_tft_espi_t *)lv_display_get_driver_data(disp);
	dsc->tft->setRotation(1);
	// lv_tft_espi_t* dsc = (lv_tft_espi_t*)lv_display_get_driver_data(disp);
	// dsc->tft->
	// lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565);
	// lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565A8);

    label = lv_label_create( lv_screen_active() );
	lv_obj_align(label, LV_ALIGN_TOP_LEFT, 5, 5);

    /* Create the UI */
	// create_side_by_side_lists();

	effects_group = lv_group_create();
    params_group = lv_group_create();
    presets_group = lv_group_create();

	setup_input_devices();

    lv_indev_set_group(param_selector, effects_group);
    lv_indev_set_group(value_selector, params_group);
	lv_indev_set_group(preset_selector, presets_group);

	set_scroll_callback(value_selector, param_encoder_turned);

	// lv_indev_add_event_cb(param_selector, param_edited_event, LV_EVENT_ALL, NULL);
	
	
	//  lv_indev_add_event_cb(value_selector, value_changed_event, LV_EVENT_ALL, params_group->obj_focus);

    Serial.println( "Display initialized" );
}

// Create an arc object with styling
FLASHMEM lv_obj_t * create_arc(lv_obj_t* parent, float value, int32_t size = 25){
	// Arc styling
	static lv_style_t style_arc_main;
	static lv_style_t style_arc_indicator;
	static lv_style_t style_arc_knob;
	lv_style_init(&style_arc_main);
	lv_style_init(&style_arc_indicator);
	lv_style_init(&style_arc_knob);

	lv_style_set_arc_width(&style_arc_main, 6);
	lv_style_set_arc_width(&style_arc_indicator, 6);
	lv_style_set_arc_color(&style_arc_main, lv_palette_lighten(LV_PALETTE_LIME, 4));
	lv_style_set_arc_color(&style_arc_indicator, lv_palette_darken(LV_PALETTE_BLUE,2));
	lv_style_set_bg_color(&style_arc_knob, lv_palette_darken(LV_PALETTE_BLUE, 4));
	lv_style_set_pad_all(&style_arc_knob, 2);


	lv_obj_t *arc = lv_arc_create(parent);
	lv_obj_add_style(arc, &style_arc_main, LV_PART_MAIN);
	lv_obj_add_style(arc, &style_arc_indicator, LV_PART_INDICATOR);
	lv_obj_add_style(arc, &style_arc_knob, LV_PART_KNOB);

	lv_obj_set_size(arc, size, size);
	// lv_arc_set_rotation(arc, 135);
	// lv_arc_set_bg_angles(arc, 0, 270);
	lv_arc_set_value(arc, value);

	//TODO: if touchscreen is used, should these be able to be adjusted?
	//These 2 lines make the arc non-adjustable according to the docs. Is the second one enough?
	// lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
	lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);	

	return arc;
} 

//TODO: rename and break up this function
FLASHMEM void create_effect_lists(Effect *effects_chain[], size_t length){
	chain_length = length;
	    /* Create a parent container */
    lv_obj_t * main_container = lv_obj_create(lv_scr_act());
    lv_obj_set_size(main_container, 470, 230);  // Set size of the container
    lv_obj_align(main_container, LV_ALIGN_TOP_MID, 0, 25);  // Center the container on the screen
    lv_obj_set_layout(main_container, LV_LAYOUT_FLEX);  // Set the container layout to flex
    lv_obj_set_flex_flow(main_container, LV_FLEX_FLOW_ROW);  // Set the flex flow to row
    lv_obj_set_flex_align(main_container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START); // Align items to start

    // Create the first lists we'll add the effects to
    effects_list = lv_list_create(main_container);
    lv_obj_set_size(effects_list, 180, 200);

    lv_list_add_text(effects_list, "Effects");

	params_lists = new lv_obj_t*[chain_length];
	for (size_t i = 0; i < chain_length; i++)
	{
		lv_obj_t* list_button = lv_list_add_btn(effects_list, NULL, effects_chain[i]->name.begin());
		// lv_obj_set_style_bg_color(list_button, lv_color_hex(0xFF0000), 0);
		Effect* effect = effects_chain[i];
		list_button->user_data = effect;

		// Create the arc for the effect wet level
		create_arc(list_button, DEFAULT_WET);

		//Add to group 1 so that it can be scrolled with the encoder
		lv_group_add_obj(effects_group, list_button);

		//Set up params list
		params_lists[i] = lv_list_create(main_container);
		lv_list_add_text(params_lists[i], "Params");
		lv_obj_set_size(params_lists[i], 200, 200);  // Set size of the second list to fill the container
		lv_obj_set_flex_grow(params_lists[i], 1);

		//TODO: maybe blink actively edited parameter?
		static lv_style_t param_button_editing_style;
		lv_style_init(&param_button_editing_style);
		// lv_style_set_outline_width(&param_button_editing_style, 2);
		lv_style_set_text_decor(&param_button_editing_style, LV_TEXT_DECOR_UNDERLINE);

		for (size_t param_index = 0; param_index < effect->num_params; param_index++)
		{
			Param* param = &effect->params[param_index];
			auto param_button = lv_list_add_btn(params_lists[i], LV_SYMBOL_FILE, param->name.begin());
			lv_obj_add_style(param_button, &param_button_editing_style, LV_STATE_EDITED);

			lv_obj_set_user_data(param_button, &effect->params[param_index]);
			//Add to group 2 so that it can be scrolled with the encoder
			lv_group_add_obj(params_group, param_button);


			lv_obj_t* value_label = lv_label_create(param_button);
			lv_label_set_text_fmt(value_label, "%.1f%s", param->current_value, param->unit.begin());

			create_arc(param_button, param->get_as_percentage());

			lv_obj_add_event_cb(param_button, param_selected_event, LV_EVENT_CLICKED, NULL); 
				

		}

		// Only the selected effect's parameter list is shown
		if(i > 0) lv_obj_add_flag(params_lists[i], LV_OBJ_FLAG_HIDDEN);

		lv_obj_add_event_cb(list_button, bypass_event, LV_EVENT_CLICKED, NULL); 			// Encoder click: toggle bypass
		lv_obj_add_event_cb(list_button, effect_focused_event, LV_EVENT_FOCUSED, NULL); 	//Add navigation with encoders
	}


    /* Create the presets list */
    presets_list = lv_list_create(lv_scr_act());
	
    lv_obj_set_size(presets_list, 470, 50);
	lv_obj_align(presets_list, LV_ALIGN_BOTTOM_MID, 0, -10);

	lv_obj_set_flex_flow(presets_list, LV_FLEX_FLOW_ROW);

	//TODO: grey out empty presets
	for (size_t i = 0; i < 4; i++)
	{
		lv_obj_t* preset_btn =  lv_list_add_btn(presets_list, LV_SYMBOL_FILE, ("P " + String(i)).begin());
		lv_group_add_obj(presets_group, preset_btn);
		lv_obj_set_width(preset_btn, lv_pct(25));

		lv_obj_add_event_cb(preset_btn, preset_pressed, LV_EVENT_SHORT_CLICKED, NULL);
		lv_obj_add_event_cb(preset_btn, preset_long_press, LV_EVENT_LONG_PRESSED, NULL); 

		lv_obj_set_user_data(preset_btn, &preset_bank.presets[i]);
	}
}


// Helper function for updating the value label of a parameter
void update_value_label(lv_obj_t* obj, float new_value, char* unit_str){
	lv_obj_t* label = lv_obj_get_child_by_type(obj, -1, &lv_label_class); // oldest label is the param name, youngest should be the value
	if (label) {
		// TODO: don't put a space before '%' in the unit string
		 lv_label_set_text_fmt(label, "%.1f%s", new_value, unit_str);
	}
	else{
		LV_LOG_USER("Value label object not found");
	}

}

//Finds the arc type child on the given object and updates its knob value
void update_arc(lv_obj_t* obj, float value){
	lv_obj_t* arc = lv_obj_get_child_by_type(obj, 0, &lv_arc_class);

	//Update arc
	if (arc) {
		lv_arc_set_value(arc, value);
	}
	else{
		LV_LOG_USER("Arc object not found");
	}
	
}

// Set all the knob and text values to show the current param values for the effects chain
void apply_param_values_to_knobs(){
	LV_LOG_USER("Applying parameter values...");

	for (size_t effect_index = 0; effect_index < lv_obj_get_child_count_by_type(effects_list, &lv_list_button_class); effect_index++) {
		lv_obj_t *effect_btn = lv_obj_get_child_by_type(effects_list, effect_index, &lv_list_button_class);
		if (effect_btn == NULL) {
			LV_LOG_USER("Effect button at index %d is NULL", (int)effect_index);
			continue;
		}

		Effect* effect = (Effect *)effect_btn->user_data;
		LV_LOG_USER("Setting knobs for effect: %d (%s)", effect_index, effect->name.begin());

		float dry_wet_value = effect->bypass ? 0 : effect->params[0].current_value;
		update_arc(effect_btn, dry_wet_value);

		// LV_LOG_USER("Dry/Wet Value: %f", dry_wet_value);

		for (size_t param_index = 0; param_index < lv_obj_get_child_count_by_type(params_lists[effect_index], &lv_list_button_class); param_index++)
		{
			lv_obj_t * param_btn = lv_obj_get_child_by_type(params_lists[effect_index], param_index, &lv_list_button_class);
			Param* param = (Param*)param_btn->user_data;
			update_value_label(param_btn, param->current_value, param->unit.begin());
			update_arc(param_btn, param->current_value);
		}
	}


	
}