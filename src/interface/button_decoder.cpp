#include <interface/button_decoder.h>
// #define LOG_BUTTON_PRESSES

void (*short_press_release_function_map[8])() = { nullptr };
void (*long_press_start_function_map[8])() = { nullptr };

uint8_t current_button_code = 0;
uint8_t last_button_code = 0;
bool any_button_pressed = false;
bool prev_any_button_pressed = false;
uint32_t last_press_start_time = 0;
bool long_press_ran = false;

void setup_decoder()
{
	LV_LOG_USER("Setting up decoder...");
    pinMode(BUTTON_MTX_A, INPUT);
    pinMode(BUTTON_MTX_B, INPUT);
    pinMode(BUTTON_MTX_C, INPUT);
    pinMode(BUTTON_MTX_GS, INPUT);

	LV_LOG_USER("Decoder setup finished");
}

inline uint8_t read_button_code()
{
	return digitalRead(BUTTON_MTX_A) + (digitalRead(BUTTON_MTX_B) << 1) +  (digitalRead(BUTTON_MTX_C) << 2);
}

inline void call_attached_func_button_released(uint8_t button_index, uint32_t press_length_ms)
{
	callbackFunction short_press_func = short_press_release_function_map[button_index];
	callbackFunction long_press_func = long_press_start_function_map[button_index];

	if(short_press_func == nullptr) return;

	if(long_press_func == nullptr || press_length_ms < LONG_PRESS_TIME)
	{
		short_press_func();
	}
}

inline void call_attached_func_long_press(uint8_t button_index)
{
	callbackFunction long_press_func = long_press_start_function_map[button_index];
	if(long_press_func == nullptr) return;
	long_press_func();
}


void decoder_tick()
{
	if(digitalRead(BUTTON_MTX_GS))
	{
		//GS is high, so no button is pressed

		if(any_button_pressed) //this is the end of the keypress
		{
			uint32_t press_length = millis() - last_press_start_time;
			#ifdef LOG_BUTTON_PRESSES
			Serial.printf("End of press, length: %d\n", press_length);
			#endif
			call_attached_func_button_released(last_button_code, press_length);
		}

		any_button_pressed = false;
		current_button_code = 0;
		last_button_code = 0;
		long_press_ran = false;

		return;
	}

	current_button_code = read_button_code();

	#ifdef LOG_BUTTON_PRESSES
	Serial.printf("Pressed button %d\n", current_button_code);
	#endif

	if(any_button_pressed == false) //this is the first detected edge
	{
		any_button_pressed = true;
		last_press_start_time = millis();
	}

	uint32_t press_length = millis() - last_press_start_time;
	if(long_press_ran == false && press_length > LONG_PRESS_TIME)
	{
		call_attached_func_long_press(current_button_code);
		long_press_ran = true;
	}

	last_button_code = current_button_code;
	
}

void decoder_attach_click(uint8_t button_index, callbackFunction callback)
{
	short_press_release_function_map[button_index] = callback;

	Serial.printf("Set click function for button %d: %d\n", button_index, short_press_release_function_map[button_index]);
}

void decoder_attach_long_press(uint8_t button_index, callbackFunction callback)
{
	long_press_start_function_map[button_index] = callback;
}