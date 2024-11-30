#include "effect.h"

Effect::Effect(String name, Param params[], size_t num_params, AudioStream *chain_start, AudioStream *chain_end) : name(name), params(params), num_params(num_params), chain_start(chain_start), chain_end(chain_end)
{
	dry_wet_param = Param("Dry/Wet", DEFAULT_WET, 5, 100, 5, std::bind(&Effect::update_dry_wet, this, std::placeholders::_1), "%");
	// // this->params[0] = dry_wet_param;
	// // this->params = new Param[num_params];
	// // for (size_t i = 0; i < num_params; i++)
	// // {
	// // 	// this->params[i] = params[i];
	// // }
	update_dry_wet(dry_wet_param.current_value);
	input_amp.gain(1.0);

	// init_connections();
}

void Effect::init_connections()
{
	// dry_patchcord = AudioConnection(input_amp, 0, this->dry_wet_mixer, 0);
	// wet_patchcord_in = AudioConnection(input_amp, 0, *chain_start, 0);
	// wet_patchcord_out = AudioConnection(*chain_end, 0, this->dry_wet_mixer, 1);

	// this->chain_start = &input_amp;
	// this->chain_end = &this->dry_wet_mixer;
}

void Effect::update_dry_wet(float new_value)
{
	dry_wet_mixer.gain(0, 1 - new_value / 100.0);
	dry_wet_mixer.gain(1, new_value / 100.0);
}

float Effect::toggle_bypass()
{
	return set_bypass(!bypass);
}

float Effect::set_bypass(bool bypass)
{
	this->bypass = bypass;
	float value = bypass ? 0 : params[0].current_value;
	update_dry_wet(value);
	statusbar_log("Set wet to " + String(bypass ? 0 : value) + "%");
	return value;
}

void Effect::next_param(int change = 1)
{
	selected_param = (selected_param + change + num_params) % num_params;
	statusbar_log("Selected parameter: " + String(params[selected_param].name));
}

void Effect::increment()
{
	params[selected_param].increment();
}

void Effect::decrement()
{
	params[selected_param].decrement();
}
void Effect::change_param(int steps)
{
	Param p = params[selected_param];
	p.change(steps);
	statusbar_log(String(p.name) + ": " + String(p.current_value));
	params[selected_param] = p;
}
