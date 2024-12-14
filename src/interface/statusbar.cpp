#include "statusbar.h"

lv_obj_t *status_bar; // Main container for the status bar
lv_obj_t *preset_label; // Styled label for the active preset
lv_obj_t *mix_arc;     // Arc for Mix volume
lv_obj_t *headphone_arc; // Arc for Headphone volume

lv_style_t preset_label_style;

void update_log_message(const char *message) {
    lv_label_set_text(log_label, message);
}

void update_preset_number(String preset_name) {
    lv_label_set_text(preset_label, preset_name.begin());
}

FLASHMEM void create_status_bar() {
    // Create the status bar container
    status_bar = lv_obj_create(lv_scr_act());
    lv_obj_set_size(status_bar, lv_pct(100), LV_SIZE_CONTENT);
    lv_obj_align(status_bar, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_flex_flow(status_bar, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(status_bar, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);


    // Log message area
    log_label = lv_label_create(status_bar);
    lv_label_set_long_mode(log_label, LV_LABEL_LONG_SCROLL_CIRCULAR); // Scrolling log message
    // lv_obj_set_width(log_label, lv_pct(50));
    lv_obj_set_flex_grow(log_label, 1);
    lv_obj_set_style_pad_all(status_bar, 5, LV_PART_MAIN);
    lv_label_set_text(log_label, "Ready"); // Initial log message

    // Preset number (styled label)
    lv_style_init(&preset_label_style);

    // Set the font to bold (ensure the font is included in your project)
    // lv_style_set_text_color(&preset_label_style, lv_color_white()); // Text color

    // Add an outline
    // lv_style_set_outline_width(&preset_label_style, 3);           // Outline thickness

    lv_obj_t *preset_num_container = lv_obj_create(status_bar);
    lv_obj_set_size(preset_num_container, LV_SIZE_CONTENT, LV_SIZE_CONTENT); // Container for the Mix arc
    lv_obj_set_flex_flow(preset_num_container, LV_FLEX_FLOW_ROW);

    preset_label = lv_label_create(preset_num_container);
    lv_obj_set_width(preset_num_container, lv_pct(14));
    lv_obj_add_style(preset_label, &preset_label_style, LV_PART_MAIN); 
    lv_obj_set_flex_align(preset_num_container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // lv_obj_add_style(preset_label, &lv_style_plain_color, 0);
	lv_style_set_text_font(&preset_label_style, &symbols_large);
    lv_label_set_text(preset_label, "1" );

    // Mix volume arc

    lv_obj_t *mix_container = lv_obj_create(status_bar);
    lv_obj_set_size(mix_container, LV_SIZE_CONTENT, LV_SIZE_CONTENT); // Container for the Mix arc
    lv_obj_set_flex_flow(mix_container, LV_FLEX_FLOW_ROW);


    lv_obj_t *mix_label = lv_label_create(mix_container);
	lv_obj_add_style(mix_label, &preset_label_style, LV_PART_MAIN); 


    lv_label_set_text(mix_label, SYMBOL_SLIDERS);
    mix_arc = create_arc(mix_container, 50, 28);


    // Headphone volume arc
    lv_obj_t *headphone_container = lv_obj_create(status_bar);
    lv_obj_set_size(headphone_container, LV_SIZE_CONTENT, LV_SIZE_CONTENT); // Container for the Headphone arc
    lv_obj_set_flex_flow(headphone_container, LV_FLEX_FLOW_ROW);

    lv_obj_t *headphone_label = lv_label_create(headphone_container);
	lv_obj_add_style(headphone_label, &preset_label_style, LV_PART_MAIN); 
	lv_label_set_text(headphone_label, SYMBOL_HEADPHONES);

    headphone_arc = create_arc(headphone_container, 50, 28);
}

void statusbar_set_headphone_arc(float new_value)
{
    lv_arc_set_value(headphone_arc, new_value*100);
}

void statusbar_set_mix_arc(float new_value)
{
    lv_arc_set_value(mix_arc, new_value*100);
}

void statusbar_set_preset_num(uint8_t preset_index)
{
     lv_label_set_text_fmt(preset_label, "%d", preset_index);
}