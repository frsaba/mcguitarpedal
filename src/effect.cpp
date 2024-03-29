#include <iterator>
#include <effect.h>

Effect::Effect(Param params[], int num_params, AudioStream* chain_start, AudioStream* chain_end): 
	params(params), num_params(num_params), chain_start(chain_start), chain_end(chain_end)  {
}

void Effect::next_param(int change = 1){
	selected_param = (selected_param + 1) % num_params;
	Serial.println("Selected parameter: " + String(params[selected_param].name));
}

void Effect::increment(){
	params[selected_param].increment();
}

void Effect::decrement(){
	params[selected_param].decrement();
}
void Effect::change_param(int steps){
	params[selected_param].change(steps);
}
