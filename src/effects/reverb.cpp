#include <functional>
#include "param.h"
#include <Arduino.h>
#include <Audio.h>
#include "effect.h"

#define DEFAULT_REVERB_TIME_MS 200
#define DEFAULT_REVERB_GAIN 0.3

class Reverb : public Effect
{
private:
    Param reverb_params[3]{
        dry_wet_param,
        Param("Reverb time", DEFAULT_REVERB_TIME_MS, 100, 2000, 100, std::bind(&Reverb::update_reverb_time, this, std::placeholders::_1), "ms"),
        Param("Gain", 0.2, 0, 0.7, 0.1, std::bind(&Reverb::update_gain, this, std::placeholders::_1))};

    AudioAmplifier amp;       // xy=532.5999908447266,258.19998359680176
    AudioEffectReverb reverb; // xy=660.6000213623047,258.1999683380127
    AudioConnection patchCord;

public:
    void update_reverb_time(float new_ms)
    {
        Serial.println("Time: " + String(new_ms));
        reverb.reverbTime(new_ms / 1000);
    }

    void update_gain(float new_gain)
    {
        Serial.println("Gain: " + String(new_gain));
        amp.gain(new_gain);
    }

    Reverb() : Effect(
                   "Reverb",
                   reverb_params,
                   3,
                   &amp,
                   &reverb),
                   patchCord(amp, 0, reverb, 0)
    {
        // init_connections();
        amp.gain(DEFAULT_REVERB_GAIN);
        update_reverb_time(20);
    }
};
