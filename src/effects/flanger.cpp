#include <functional>
#include "param.h"
#include <Arduino.h>
#include <Audio.h>
#include "effect.h"

#define DEFAULT_FLANGE_LENGTH 16
#define MAX_FLANGE_DELAY_LENGTH 30
#define DEFAULT_OFFSET (MAX_FLANGE_DELAY_LENGTH/4)

#define DEFAULT_DEPTH 10
#define DEFAULT_RATE 1

class Flanger : public Effect
{
private:
    Param params[4]{
        dry_wet_param,
        Param("Offset", DEFAULT_OFFSET, 1, MAX_FLANGE_DELAY_LENGTH, 1, std::bind(&Flanger::update_offset, this, std::placeholders::_1)),
        Param("Depth", DEFAULT_DEPTH, 5, MAX_FLANGE_DELAY_LENGTH, 1, std::bind(&Flanger::update_depth, this, std::placeholders::_1)),
		Param("Rate", DEFAULT_RATE, 0.5, 10, 0.5, std::bind(&Flanger::update_rate, this, std::placeholders::_1), "Hz")};

    AudioEffectFlange flange; // xy=660.6000213623047,258.1999683380127
    AudioConnection patchCord;
	short delayline[MAX_FLANGE_DELAY_LENGTH * AUDIO_BLOCK_SAMPLES];

public:
    void update_offset(float offset)
    {
        // Serial.println("Voices: " + String(offset));
		flange.voices(offset * AUDIO_BLOCK_SAMPLES, params[2].current_value * AUDIO_BLOCK_SAMPLES, params[3].current_value);
    }

    void update_depth(float new_length)
    {
        // Serial.println("Length: " + String(new_length));
        flange.begin(delayline, new_length * AUDIO_BLOCK_SAMPLES, params[1].current_value * AUDIO_BLOCK_SAMPLES, params[2].current_value * AUDIO_BLOCK_SAMPLES, params[3].current_value);
    }

	void update_rate(float new_rate)
    {
        // Serial.println("Rate: " + String(new_rate));
        flange.voices(params[1].current_value * AUDIO_BLOCK_SAMPLES, params[2].current_value * AUDIO_BLOCK_SAMPLES, new_rate);
    }


    Flanger() : Effect(
                   "Flanger",
                   params,
                   4,
                   &flange,
                   &flange),
                   patchCord(input_amp, 0, flange, 0)
    {
        // init_connections();
        input_amp.gain(1);
		flange.begin(delayline, DEFAULT_FLANGE_LENGTH * AUDIO_BLOCK_SAMPLES, DEFAULT_OFFSET * AUDIO_BLOCK_SAMPLES, DEFAULT_DEPTH * AUDIO_BLOCK_SAMPLES, DEFAULT_RATE);
    }
};
