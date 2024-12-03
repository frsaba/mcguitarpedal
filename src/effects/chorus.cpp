#include <functional>
#include "param.h"
#include <Arduino.h>
#include <Audio.h>
#include "effect.h"

#define DEFAULT_VOICES 3
#define DEFAULT_CHORUS_LENGTH 4
#define MAX_CHORUS_LENGTH 10

#define MAX_CHORUS_DELAY_LENGTH (MAX_CHORUS_LENGTH*AUDIO_BLOCK_SAMPLES)

class Chorus : public Effect
{
private:
    Param params[3]{
        dry_wet_param,
        Param("Voices", DEFAULT_VOICES, 1, 5, 1, std::bind(&Chorus::update_voices, this, std::placeholders::_1), CUSTOM_SYMBOL_USERS_LINE),
        Param("Length", DEFAULT_CHORUS_LENGTH, 1, MAX_CHORUS_LENGTH, 1, std::bind(&Chorus::update_length, this, std::placeholders::_1), CUSTOM_SYMBOL_ARROWS_LEFT_RIGHT_TO_LINE)};

    AudioEffectChorus chorus; // xy=660.6000213623047,258.1999683380127
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
        chorus.begin(delayline, new_length * AUDIO_BLOCK_SAMPLES, params[1].current_value);
    }

    Chorus() : Effect(
                   "Chorus",
                   params,
                   3,
                   &chorus,
                   &chorus)
                //    patchCord(input_amp, 0, chorus, 0)
    {
        // init_connections();
        input_amp.gain(1);
		chorus.begin(delayline, DEFAULT_CHORUS_LENGTH * AUDIO_BLOCK_SAMPLES, DEFAULT_VOICES);
    }
};
