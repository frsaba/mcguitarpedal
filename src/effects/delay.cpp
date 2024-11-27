#include <functional>
#include "param.h"
#include <Arduino.h>
#include <Audio.h>
#include "effect.h"
#define DEFAULT_DELAY_FEEDBACK 0.2
#define DEFAULT_DELAY_MS 100

//TODO: feedback chain
class Delay : public Effect
{
private:
    Param parameters[3]{
        dry_wet_param,
        Param("Delay", DEFAULT_DELAY_MS, 10, 600, 10, std::bind(&Delay::update_delay_time, this, std::placeholders::_1), "ms"),
        Param("Feedback", DEFAULT_DELAY_FEEDBACK, 0, 0.9, 0.1, std::bind(&Delay::update_feedback, this, std::placeholders::_1))};

    AudioEffectDelay delay;
    AudioMixer4 input_mixer;
    AudioFilterBiquad filter;     
    AudioConnection patchCord4;
    AudioConnection patchCord5;
    AudioConnection patchCord6;
    AudioConnection patchCord7;

public:
    void update_delay_time(float new_ms)
    {
        // Serial.println("Delay: " + String(new_ms));
        delay.delay(0, new_ms);
        // delay.delay(1, new_ms * 2);
        // delay.delay(2, new_ms * 3);
        // delay.delay(3, new_ms * 4);
    }

    void update_feedback(float new_feedback)
    {
        input_mixer.gain(0, new_feedback);
        // Serial.println("Feedback: " + String(new_feedback));
    }

    Delay() : Effect(
                  "Delay",
                  parameters,
                  3,
                  &input_mixer,
                  &delay),
              patchCord4(input_mixer, 0, delay, 0), patchCord5(delay, 0, filter, 0), patchCord6(filter, 0, input_mixer, 1)
    {
        // init_connections();

        input_mixer.gain(0, 1);
        input_mixer.gain(1, DEFAULT_DELAY_FEEDBACK);
        filter.setLowpass(0,600,0.7);
        update_delay_time(20);
    }
};
