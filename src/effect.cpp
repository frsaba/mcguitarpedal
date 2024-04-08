#include <iterator>
#include <effect.h>
#include <display.h>

Effect::Effect(Param params[], int num_params, AudioStream* chain_start, AudioStream* chain_end): 
	params(params), num_params(num_params), chain_start(chain_start), chain_end(chain_end)  {
}

void Effect::next_param(int change = 1){
	selected_param = (selected_param + change + num_params) % num_params;
	displayText("Selected parameter: " + String(params[selected_param].name));
}

void Effect::increment(){
	params[selected_param].increment();
}

void Effect::decrement(){
	params[selected_param].decrement();
}
void Effect::change_param(int steps){
	Param p = params[selected_param];
	p.change(steps);
	displayText(String(p.name) + ": " + String(p.current_value));
	params[selected_param] = p;
}
