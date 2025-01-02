#include <functional>
#include "param.h"
#include <Arduino.h>
#include <Audio.h>
#include "effect.h"

#define DEFAULT_FREQUENCY 4

class Tremolo : public Effect
{
private:
    Param params[3]{
        dry_wet_param,
        Param("Rate", DEFAULT_FREQUENCY, 0.5, 10, 0.5, std::bind(&Tremolo::update_frequency, this, std::placeholders::_1), CUSTOM_SYMBOL_WAVE_SQUARE, "Hz"),
        Param("Depth", 1, 0.1, 1, 0.1, std::bind(&Tremolo::update_depth, this, std::placeholders::_1), CUSTOM_SYMBOL_UP_DOWN)};

    AudioSynthWaveform wave;       // xy=532.5999908447266,258.19998359680176
    AudioEffectMultiply multiply; // xy=660.6000213623047,258.1999683380127
    AudioConnection patchCord;

public:
    void update_frequency(float new_freq)
    {
        Serial.println("Rate: " + String(new_freq) + " Hz");
        wave.frequency(new_freq);
    }

    void update_depth(float new_depth)
    {
        Serial.println("Depth: " + String(new_depth));
        wave.offset(1 - new_depth / 2);
		wave.amplitude(new_depth / 2);
    }

    Tremolo() : Effect(
                   "Tremolo",
                   params,
                   3,
                   &multiply,
                   &multiply),
                   patchCord(wave, 0, multiply, 1)
    {
        update_frequency(DEFAULT_FREQUENCY);
		update_depth(1);

		wave.begin(WAVEFORM_SINE);
    }
};
