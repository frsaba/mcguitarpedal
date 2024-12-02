#pragma once
// #include <EEPROM.h>

#include <Arduino.h>

// TODO: make these dynamic / make sure CHAIN_LENGTH is correct always
#define CHAIN_LENGTH 5
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

extern preset_bank_t preset_bank;

size_t preset_get_active_index();
preset_data_t preset_get_active();

FLASHMEM size_t serialize_presets(const preset_bank_t& presets, bool write_to_eeprom = false);

FLASHMEM void load_presets_from_eeprom(preset_bank_t* bank);
void apply_preset_values(effect_data_t effect_values[], Effect** effect_chain,  size_t num_effects);

preset_data_t effects_to_preset_data(String preset_name, Effect *effects_chain[]);

Effect* findEffectByName(String name, Effect** effect_chain, size_t num_effects);
Param* findParamByName(String name, Effect* effect);