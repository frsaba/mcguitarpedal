#pragma once
#include <interface/display.h>

extern lv_obj_t *log_label;  // Label for log messages

template <typename... Args>
void statusbar_log_fmt(const char* format, Args... args) {
    lv_label_set_text_fmt(log_label, format, args...);
    Serial.printf(format, args...);
}

void statusbar_log(const String &s);
void create_status_bar();

void statusbar_set_headphone_arc(float new_value);
void statusbar_set_mix_arc(float new_value);
void statusbar_set_preset_num(uint8_t preset_index);
