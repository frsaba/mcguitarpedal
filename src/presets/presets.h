// #include <EEPROM.h>
#include <effects/effect.h>
#include <Arduino.h>

typedef struct{
	String effect_name;
	String param_name;
	float param_value;
} preset_data;

void save_preset(Effect** effect_chain, size_t num_effects);

void load_preset(Effect** effect_chain,  size_t num_effects);