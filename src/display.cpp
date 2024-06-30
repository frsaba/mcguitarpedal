#include "display.h"
#include <lvgl.h>
#include "encoder_input.h"

#if LV_USE_TFT_ESPI
#include <TFT_eSPI.h>
#endif


lv_obj_t *label;


void displayText(const String &s)
{
	lv_label_set_text( label, s.begin() );
    Serial.println(s);
}

/*Set to your screen resolution and rotation*/
#define TFT_HOR_RES   320
#define TFT_VER_RES   480
#define TFT_ROTATION  LV_DISPLAY_ROTATION_270

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 10 * (LV_COLOR_DEPTH / 8))
uint32_t draw_buf[DRAW_BUF_SIZE / 4];

#if LV_USE_LOG != 0
void my_print( lv_log_level_t level, const char * buf )
{
    // LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif

/* LVGL calls it when a rendered image needs to copied to the display*/
// void my_disp_flush( lv_display_t *disp, const lv_area_t *area, uint8_t * px_map)
// {
//     uint32_t w = (area->x2 - area->x1 + 1);
//     uint32_t h = (area->y2 - area->y1 + 1);

//     tft.setAddrWindow(area->x1, area->y1, w, h);
//     tft.pushColors((uint16_t *)&px_map, w * h);
//     lv_disp_flush_ready(disp);
// }

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

static lv_group_t * group1;
static lv_group_t * group2;

static lv_obj_t * list1;
// static lv_obj_t * list2;

// typedef struct {
//     TFT_eSPI * tft;
// } lv_tft_espi_t;

void init_display()
{
    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.println( LVGL_Arduino );

    lv_init();

    /*Set a tick source so that LVGL will know how much time elapsed. */
    lv_tick_set_cb(my_tick);

    /* register print function for debugging */
#if LV_USE_LOG != 0
    lv_log_register_print_cb( my_print );
#endif

    lv_display_t * disp;
    /*TFT_eSPI can be enabled lv_conf.h to initialize the display in a simple way*/
    disp = lv_tft_espi_create(TFT_HOR_RES, TFT_VER_RES, draw_buf, sizeof(draw_buf));
    lv_display_set_rotation(disp, TFT_ROTATION);
	// lv_tft_espi_t* dsc = (lv_tft_espi_t*)lv_display_get_driver_data(disp);
	// dsc->tft->
	// lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565);
	// lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565A8);


    /*Initialize the (dummy) input device driver*/
    // lv_indev_t * indev = lv_indev_create();
    // lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /*Touchpad should have POINTER type*/
    // lv_indev_set_read_cb(indev, my_touchpad_read);

    /* Create a simple label
     * ---------------------
     lv_obj_t *label = lv_label_create( lv_screen_active() );
     lv_label_set_text( label, "Hello Arduino, I'm LVGL!" );
     lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );

     * Try an example. See all the examples
     *  - Online: https://docs.lvgl.io/master/examples.html
     *  - Source codes: https://github.com/lvgl/lvgl/tree/master/examples
     * ----------------------------------------------------------------

     lv_example_btn_1();

     * Or try out a demo. Don't forget to enable the demos in lv_conf.h. E.g. LV_USE_DEMOS_WIDGETS
     * -------------------------------------------------------------------------------------------

     lv_demo_widgets();
     */

    label = lv_label_create( lv_screen_active() );
    // lv_label_set_text( label, String("Hello Arduino, I'm LVGL!" + String(LV_SYMBOL_AUDIO )).begin());
    // lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );
	// static lv_style_t label_style;
	// lv_style_init(&label_style);
	// lv_style_set_bg_color(&label_style, lv_color_hex(0x1F1F1F));
	// lv_style_set_bg_opa(&label_style, LV_OPA_50);
	// lv_style_set_border_width(&label_style, 2);
	// lv_style_set_border_color(&label_style, lv_color_black());
	// lv_style_set_text_color(&label_style, lv_color_white());
	// lv_style_set_text_font(&label_style, &lv_font_montserrat_24);
	// lv_obj_add_style(label, &label_style, 0);

    /* Create the UI */
	// create_side_by_side_lists();

	group1 = lv_group_create();
    group2 = lv_group_create();
	setup_input_devices();

    lv_indev_set_group(effect_selector, group1);
    lv_indev_set_group(param_selector, group2);

    Serial.println( "Setup done" );
}

//Called when effect encoder button is pressed. Toggles the effect bypass
static void bypass_event(lv_event_t * e)
{
    lv_obj_t * target = (lv_obj_t *)lv_event_get_target(e);
	Effect* effect = (Effect *)target->user_data;

	// LV_UNUSED(obj);
	float new_value = effect->toggle_bypass();
	LV_LOG_USER("Clicked: %s", effect->name.begin());
	lv_obj_t* arc = lv_obj_get_child(target, 1); // First child is the label, second should be the arc

	if (lv_obj_check_type(arc, &lv_arc_class)) {
		
		lv_arc_set_value(arc, new_value);
		// lv_obj_invalidate(arc); // Force redraw
	}
	else{
		LV_LOG_USER("Arc object not found");
	}


}
static size_t chain_length;
lv_obj_t** params_lists;
// Called when the effect is navigated to; Hides the old params list and shows the new one instead.
static void effect_focused_event(lv_event_t * e)
{
    lv_obj_t * target = (lv_obj_t *)lv_event_get_target(e);
	Effect* effect = (Effect *)target->user_data;
	int effect_index = lv_obj_get_index(target) - 1;

	if(effect_index < 0){
		LV_LOG_ERROR("Could not find focused effect index");
		return;	
	}

	LV_LOG_USER("Clicked: %s %d", effect->name.begin(), effect_index);

	for (size_t i = 0; i < chain_length; i++)
	{
		if(i == static_cast<size_t>(effect_index)) lv_obj_remove_flag(params_lists[i], LV_OBJ_FLAG_HIDDEN);
		else lv_obj_add_flag(params_lists[i], LV_OBJ_FLAG_HIDDEN);
	}
}


void create_effect_lists(Effect *effects_chain[], size_t length){
	chain_length = length;
	    /* Create a parent container */
    lv_obj_t * container = lv_obj_create(lv_scr_act());
    lv_obj_set_size(container, 470, 240);  // Set size of the container
    lv_obj_center(container);  // Center the container on the screen
    lv_obj_set_layout(container, LV_LAYOUT_FLEX);  // Set the container layout to flex
    lv_obj_set_flex_flow(container, LV_FLEX_FLOW_ROW);  // Set the flex flow to row
    lv_obj_set_flex_align(container, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START); // Align items to start

    /* Create the first list */
    list1 = lv_list_create(container);
    lv_obj_set_size(list1, 150, 200);  // Set size of the first list

    /* Add items to the first list */
    lv_list_add_text(list1, "Effects");

	params_lists = new lv_obj_t*[chain_length];
	for (size_t i = 0; i < chain_length; i++)
	{
		lv_obj_t* list_button = lv_list_add_btn(list1, NULL, effects_chain[i]->name.begin());
		Effect* effect = effects_chain[i];
		list_button->user_data = effect;
		// lv_obj_set_size(list_button, 100, 40);
		// lv_obj_align(list_button, LV_ALIGN_CENTER, 0, 0); // Align the button to the center

		// Create an arc
		lv_obj_t *arc = lv_arc_create(list_button);

		// lv_obj_set_pos(arc, 60, 60);
		lv_obj_set_size(arc, 25, 25);
		// lv_arc_set_rotation(arc, 135);
		// lv_arc_set_bg_angles(arc, 0, 270);
		lv_arc_set_value(arc, DEFAULT_WET);
		lv_obj_set_style_arc_width(arc, 6, LV_PART_MAIN );
		lv_obj_set_style_arc_width(arc, 6, LV_PART_INDICATOR);
		lv_obj_set_style_arc_color(arc, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);
		lv_obj_set_style_arc_color(arc, lv_palette_darken(LV_PALETTE_BLUE, 3), LV_PART_INDICATOR);
		lv_obj_set_style_bg_color(arc, lv_palette_darken(LV_PALETTE_BLUE, 10), LV_PART_KNOB);
		lv_obj_set_style_pad_all(arc, 2, LV_PART_KNOB );

		lv_group_add_obj(group1, list_button);

		params_lists[i] = lv_list_create(container);
		lv_list_add_text(params_lists[i], "Params");
		lv_obj_set_size(params_lists[i], 200, 200);  // Set size of the second list to fill the container
		lv_obj_set_flex_grow(params_lists[i], 1);
		for (size_t param_index = 0; param_index < effect->num_params; param_index++)
		{
			Param* param = &effect->params[param_index];
			auto param_button = lv_list_add_btn(params_lists[i], LV_SYMBOL_FILE, param->name.begin());

			lv_group_add_obj(group2, param_button);
		}

		if(i > 0) lv_obj_add_flag(params_lists[i], LV_OBJ_FLAG_HIDDEN);

		//Add navigation with encoders
		lv_obj_add_event_cb(list_button, bypass_event, LV_EVENT_CLICKED, NULL);
		lv_obj_add_event_cb(list_button, effect_focused_event, LV_EVENT_FOCUSED, NULL);
	}


    /* Create the second list */
    // list2 = lv_list_create(container);
    // lv_obj_set_size(list2, 200, 200);  // Set size of the second list to fill the container
    // lv_obj_set_flex_grow(list2, 1);  // Set the second list to grow and take up remaining space

    /* Add items to the second list */
    // lv_list_add_text(list2, "List 2");
    // lv_list_add_btn(list2, LV_SYMBOL_FILE, "Param A");
    // lv_list_add_btn(list2, LV_SYMBOL_FILE, "Param B");
    // lv_list_add_btn(list2, LV_SYMBOL_FILE, "Param C");
}