#include <functional>
#include "param.h"
#include <Arduino.h>
#include <Audio.h>

// TODO: maybe inherit from AudioStream?
class Effect {
protected:
    Param* params;
	int num_params;
    int selected_param = 0;
public:
	AudioStream* chain_start;
	AudioStream* chain_end;

    Effect(int num_params);
	Effect(Param params[], int num_params, AudioStream* chain_start, AudioStream* chain_end);

	void next_param(int change);

	void increment();
	void decrement();
	void change_param(int steps);


};
