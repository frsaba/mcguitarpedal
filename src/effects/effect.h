#pragma once
#include <functional>
#include "param.h"
#include <Arduino.h>
#include <Audio.h>
#include <iterator>
#include <interface/display.h>
#include <presets/presets.h>

#define DEFAULT_WET 40

// TODO: maybe inherit from AudioStream?
class Effect
{
public:
	String name;

protected:
	Param *params;
	size_t num_params;
	size_t selected_param = 0;
	Param dry_wet_param;
	bool bypass = false;   

	AudioConnection dry_patchcord;
	AudioConnection wet_patchcord_in;
	AudioConnection wet_patchcord_out;

	void init_connections();

public:
	AudioMixer4 dry_wet_mixer;
	AudioAmplifier input_amp;
	AudioStream *chain_start;
	AudioStream *chain_end;

	Effect(size_t num_params);
	Effect(String name, Param params[], size_t num_params, AudioStream *chain_start, AudioStream *chain_end);

	void next_param(int change);

	void increment();
	void decrement();
	void change_param(int steps);
	void update_dry_wet(float);
	float toggle_bypass();
	float set_bypass(bool);

	friend void create_effect_lists(Effect *effects_chain[], size_t chain_length);
	friend void param_encoder_turned(int enc_diff);
	friend void load_preset(Effect** effect_chain,  size_t num_effects);
	friend preset_data_t effects_to_preset_data(String preset_name, Effect *effects_chain[]);
	friend Param* findParamByName(String name, Effect* effect);
	friend void apply_preset_values(effect_data_t effect_values[], Effect** effect_chain,  size_t num_effects);
	friend void apply_param_values_to_knobs();
	
};
