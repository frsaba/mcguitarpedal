#include <Arduino.h>

/*
 * A simple hardware test which receives audio from the audio shield
 * Line-In pins and send it to the Line-Out pins and headphone jack.
 *
 * This example code is in the public domain.
 */
#include <Encoder.h>
#include <Audio.h>
#include <Wire.h>
#include "SPI.h"
#include "ILI9488_t3.h"
#include <SD.h>
#include <SerialFlash.h>
#include "OneButton.h"

#include <delay.cpp>
#include <reverb.cpp>
#include <display.h>

#define CHAIN_LENGTH 2
Effect *effects_chain[] = {new Delay(), new Reverb()};
Effect *delay_effect = effects_chain[0];
Effect *reverb_effect = effects_chain[1];

int selected_effect_index = 0;
// GUItool: begin automatically generated code
AudioInputI2S audio_input;       // xy=334.1037826538086,172.99997901916504
AudioMixer4 dry_wet_mixer;       // xy=681.1037902832031,179
AudioAnalyzeNoteFrequency tuner; // xy=1014.1037216186523,270.00000953674316
AudioOutputI2S audio_output;     // xy=1036.1037902832031,219
AudioConnection patchCord1(audio_input, 0, dry_wet_mixer, 0);
AudioConnection patchCord2(dry_wet_mixer, 0, audio_output, 0);
AudioConnection patchCord3(dry_wet_mixer, 0, audio_output, 1);
AudioConnection patchCord4(dry_wet_mixer, tuner);
AudioConnection patchCord5(audio_input, 0, *delay_effect->chain_start, 0);
AudioConnection patchCord6(*delay_effect->chain_end, 0, *reverb_effect->chain_start, 0);
AudioConnection patchCord7(*reverb_effect->chain_end, 0, dry_wet_mixer, 1);

AudioControlSGTL5000 sgtl5000_1; // xy=256.1037902832031,460
// GUItool: end automatically generated code

// TODO: feedback delay?

const int myInput = AUDIO_INPUT_LINEIN;
// const int myInput = AUDIO_INPUT_MIC;
Encoder param_selector(31, 32);
int param_selector_prev = 0;

Encoder value_selector(29, 30);
int value_selector_prev = 0;

extern ILI9488_t3 tft;

OneButton button1(37, true);
OneButton button2(38, true);
OneButton button3(39, true);

elapsedMillis volmsec = 0;

std::vector<AudioConnection> patchCords;

void setup()
{
    // Audio connections require memory to work.  For more
    // detailed information, see the MemoryAndCpuUsage example
    AudioMemory(800);

    dry_wet_mixer.gain(0, 0.6);
    dry_wet_mixer.gain(1, 0.6);

    // //connect input to the start of the effects chain
    // AudioConnection *connection1 = new AudioConnection(audio_input, *effects_chain[0]->chain_start);
    // patchCords.push_back(*connection1);

    // // connect effects together
    // for (size_t i = 0; i < CHAIN_LENGTH - 1; i++)
    // {
    //     AudioConnection *connection = new AudioConnection(*effects_chain[i]->chain_end, *effects_chain[i + 1]->chain_start);
    //     patchCords.push_back(*connection);
    // }

    // // Connect last effect to output
    // AudioConnection *connectionLast = new AudioConnection(*effects_chain[CHAIN_LENGTH - 1]->chain_end, 0, dry_wet_mixer, 1);
    // patchCords.push_back(*connectionLast);


    sgtl5000_1.enable();
    sgtl5000_1.inputSelect(myInput);
    sgtl5000_1.adcHighPassFilterDisable();
    sgtl5000_1.volume(0.5);
    sgtl5000_1.lineInLevel(10);

    pinMode(A10, INPUT);
    button1.attachClick([]()
                        {
                            selected_effect_index = (selected_effect_index + 1) % CHAIN_LENGTH;
                            displayText("Selected effect: " + String(effects_chain[selected_effect_index]->name)); });
    button2.attachClick([]()
                        { displayText("Button 2 pressed"); });
    button3.attachClick([]()
                        { displayText("Button 3 pressed"); });

    tuner.begin(0.3);
    tft.begin();
    tft.fillScreen(ILI9488_BLACK);
    tft.setRotation(3);

    displayText("Hello");
}

void loop()
{
    if (volmsec > 150)
    {
        float vol = analogRead(A10);
        vol = vol / 1023.0;
        sgtl5000_1.volume(vol); // <-- uncomment if you have the optional
        volmsec = 0;            //     volume pot on your audio shield

        int param_selector_new = param_selector.read() / 4;
        if (param_selector_new != param_selector_prev)
        {
            effects_chain[selected_effect_index]->next_param(param_selector_new - param_selector_prev);
            param_selector_prev = param_selector_new;
        }

        int value_selector_new = value_selector.read() / 4;
        if (value_selector_new != value_selector_prev)
        {
            effects_chain[selected_effect_index]->change_param(value_selector_new - value_selector_prev);
            value_selector_prev = value_selector_new;
        }

        // if (tuner.available())
        // {
        //     displayText("Freq: " + String(tuner.read()) + " Probability: " + String(String(tuner.probability())));
        // }

        // Serial.print("all=");
        // Serial.print(AudioProcessorUsage());
        // Serial.print(",");
        // Serial.print(AudioProcessorUsageMax());
        // Serial.print("    ");
        // Serial.print("Memory: ");
        // Serial.print(AudioMemoryUsage());
        // Serial.print(",");
        // Serial.print(AudioMemoryUsageMax());
        // Serial.print("    ");
        // Serial.println();
    }
    button1.tick();
    button2.tick();
    button3.tick();
    delay(10);
}