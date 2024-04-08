#include <functional>
#include "param.h"
#include <Arduino.h>
#include <Audio.h>
#include <Effect.h>

class Delay : public Effect
{
private:
    Param parameters[2]{
        Param("Delay (ms)", 100, 5, 400, 10, std::bind(&Delay::update_delay_time, this, std::placeholders::_1)),
        Param("Feedback", 0.2, 0, 0.9, 0.1, std::bind(&Delay::update_feedback, this, std::placeholders::_1))};

    AudioEffectDelay delay; // Declare delay object here
    AudioMixer4 mixer;      // Declare mixer object here
    AudioConnection patchCord4;
    AudioConnection patchCord5;
    AudioConnection patchCord6;
    AudioConnection patchCord7;

public:
    void update_delay_time(float new_ms)
    {
        // Serial.println("Delay: " + String(new_ms));
        delay.delay(0, new_ms);
        delay.delay(1, new_ms * 2);
        delay.delay(2, new_ms * 3);
        delay.delay(2, new_ms * 4);
    }

    void update_feedback(float new_feedback)
    {
        // Serial.println("Feedback: " + String(new_feedback));
    }

    Delay() : Effect(
                  "Delay",
                  parameters,
                  2,
                  &delay,
                  &mixer),
              patchCord4(delay, 0, mixer, 0), patchCord5(delay, 1, mixer, 1), patchCord6(delay, 2, mixer, 2), patchCord7(delay, 3, mixer, 3)
    {
        mixer.gain(0, 1);
        mixer.gain(1, 0.75);
        mixer.gain(2, 0.5);
        mixer.gain(3, 0.25);
        update_delay_time(20);
    }
};
