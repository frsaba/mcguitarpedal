#pragma once
// #include <EEPROM.h>

#include <Arduino.h>

// TODO: make these dynamic / make sure CHAIN_LENGTH is correct always
#define CHAIN_LENGTH 4
#define NUM_PRESETS 4

class Effect;

typedef struct{
	String name;
	float current_value;
} param_data_t;

typedef struct{
	String name;	
	bool bypass;
	size_t num_params;
	std::vector<param_data_t> params;
} effect_data_t;

typedef struct{
	String name;
	effect_data_t effect_values[CHAIN_LENGTH];
} preset_data_t;
typedef struct{
	size_t active_preset;
	size_t num_presets;
	size_t num_effects;
	preset_data_t presets[NUM_PRESETS];
} preset_bank_t;

//FIXME: this is the rearranging i had to do because there is a circular dependency effect.h and presets.h
// (with the friend declaration)
#include <effects/effect.h>

void save_preset(Effect** effect_chain, size_t num_effects);

size_t save_presets(const preset_bank_t& presets);

preset_bank_t load_presets();

preset_data_t effects_to_preset_data(String preset_name, const Effect *effects_chain[]);
void load_preset(Effect** effect_chain,  size_t num_effects);