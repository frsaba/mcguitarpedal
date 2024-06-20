#include <functional>
#include "param.h"
#include <Arduino.h>
#include <Audio.h>
#include "effect.h"

#define DEFAULT_FLANGE_LENGTH 16
#define MAX_FLANGE_LENGTH 50

#define MAX_FLANGE_DELAY_LENGTH (30*AUDIO_BLOCK_SAMPLES)
#define DEFAULT_OFFSET MAX_FLANGE_DELAY_LENGTH/4

#define DEFAULT_DEPTH 20
#define DEFAULT_RATE 1

class Flanger : public Effect
{
private:
    Param params[4]{
        dry_wet_param,
        Param("Offset", DEFAULT_OFFSET, 1, MAX_FLANGE_DELAY_LENGTH, 10, std::bind(&Flanger::update_offset, this, std::placeholders::_1)),
        Param("Depth", DEFAULT_DEPTH, 10, MAX_FLANGE_DELAY_LENGTH, 10, std::bind(&Flanger::update_length, this, std::placeholders::_1)),
		Param("Rate", DEFAULT_RATE, 0.5, 10, 0.5, std::bind(&Flanger::update_rate, this, std::placeholders::_1))};

    AudioEffectFlange flange; // xy=660.6000213623047,258.1999683380127
    AudioConnection patchCord;
	short delayline[MAX_FLANGE_DELAY_LENGTH];

public:
    void update_offset(float offset)
    {
        Serial.println("Voices: " + String(offset));
        flange.voices(offset, params[2].current_value, params[3].current_value);
    }

    void update_length(float new_length)
    {
        Serial.println("Length: " + String(new_length));
        flange.begin(delayline, new_length, params[1].current_value, params[2].current_value, params[3].current_value);
    }

	void update_rate(float new_rate)
    {
        Serial.println("Rate: " + String(new_rate));
        flange.voices(params[1].current_value, params[2].current_value, new_rate);
    }


    Flanger() : Effect(
                   "Flanger",
                   params,
                   3,
                   &flange,
                   &flange),
                   patchCord(input_amp, 0, flange, 0)
    {
        // init_connections();
        input_amp.gain(1);
		flange.begin(delayline, DEFAULT_FLANGE_LENGTH, DEFAULT_OFFSET, DEFAULT_DEPTH, DEFAULT_RATE);
    }
};
