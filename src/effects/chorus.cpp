#include <functional>
#include "param.h"
#include <Arduino.h>
#include <Audio.h>
#include "effect.h"

#define DEFAULT_VOICES 3
#define DEFAULT_CHORUS_LENGTH 16
#define MAX_CHORUS_LENGTH 50

#define MAX_CHORUS_DELAY_LENGTH (MAX_CHORUS_LENGTH*AUDIO_BLOCK_SAMPLES)

class Chorus : public Effect
{
private:
    Param params[3]{
        dry_wet_param,
        Param("Voices", DEFAULT_VOICES, 1, 5, 1, std::bind(&Chorus::update_voices, this, std::placeholders::_1)),
        Param("Length", 16, 5, MAX_CHORUS_LENGTH, 1, std::bind(&Chorus::update_length, this, std::placeholders::_1))};

    AudioEffectChorus chorus; // xy=660.6000213623047,258.1999683380127
    AudioConnection patchCord;
	short delayline[MAX_CHORUS_DELAY_LENGTH];

public:
    void update_voices(float v)
    {
        Serial.println("Voices: " + String(v));
        chorus.voices(v);
    }

    void update_length(float new_length)
    {
        Serial.println("Length: " + String(new_length));
        chorus.begin(delayline, new_length, params[1].current_value);
    }

    Chorus() : Effect(
                   "Chorus",
                   params,
                   3,
                   &chorus,
                   &chorus),
                   patchCord(input_amp, 0, chorus, 0)
    {
        // init_connections();
        input_amp.gain(1);
		chorus.begin(delayline, DEFAULT_CHORUS_LENGTH, DEFAULT_VOICES);
    }
};