#include <functional>
#include "param.h"
#include <Arduino.h>
#include <Audio.h>
#include "effect.h"

#define DEFAULT_REVERB_ROOM_SIZE 0.3
#define DEFAULT_REVERB_DAMPING 0.4

class Reverb : public Effect
{
private:
    Param reverb_params[3]{
        dry_wet_param,
        Param("Room size", DEFAULT_REVERB_ROOM_SIZE, 0, 1, 0.1, std::bind(&Reverb::update_room_size, this, std::placeholders::_1)),
        Param("Damping", DEFAULT_REVERB_DAMPING, 0, 1, 0.1, std::bind(&Reverb::update_reverb, this, std::placeholders::_1))};

    AudioAmplifier amp;       // xy=532.5999908447266,258.19998359680176
    AudioEffectFreeverb reverb; // xy=660.6000213623047,258.1999683380127
    AudioConnection patchCord;

public:
    void update_room_size(float room_size)
    {
        // Serial.println("Time: " + String(new_ms));
        reverb.roomsize(room_size);
    }

    void update_reverb(float new_gain)
    {
        // Serial.println("Gain: " + String(new_gain));
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
        amp.gain(1.5);
        update_room_size(DEFAULT_REVERB_ROOM_SIZE);
    }
};
