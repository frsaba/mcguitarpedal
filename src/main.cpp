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

#include <delay.cpp>
#include <display.h>

Delay delay_effect = Delay();

// GUItool: begin automatically generated code
AudioInputI2S audio_input;       // xy=334.1037826538086,172.99997901916504
AudioMixer4 dry_wet_mixer;       // xy=681.1037902832031,179
AudioAnalyzeNoteFrequency tuner; // xy=1014.1037216186523,270.00000953674316
AudioOutputI2S audio_output;     // xy=1036.1037902832031,219
AudioConnection patchCord1(audio_input, 0, dry_wet_mixer, 0);
AudioConnection patchCord2(dry_wet_mixer, 0, audio_output, 0);
AudioConnection patchCord3(dry_wet_mixer, 0, audio_output, 1);
AudioConnection patchCord4(dry_wet_mixer, tuner);
AudioConnection patchCord5(audio_input, 0, *delay_effect.chain_start, 0);
AudioConnection patchCord6(*delay_effect.chain_end, 0, dry_wet_mixer, 1);
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

void setup()
{
    // Audio connections require memory to work.  For more
    // detailed information, see the MemoryAndCpuUsage example
    AudioMemory(500);

    dry_wet_mixer.gain(0, 0.5);
    dry_wet_mixer.gain(1, 0.5);

    // AudioConnection          patchCord5(audio_input, 0, *delay_effect.chain_start, 0);
    // AudioConnection          patchCord6(*delay_effect.chain_end, 0, dry_wet_mixer, 1);

    // Enable the audio shield, select input, and enable output
    sgtl5000_1.enable();
    sgtl5000_1.inputSelect(myInput);
    sgtl5000_1.adcHighPassFilterDisable();
    sgtl5000_1.volume(0.5);
    sgtl5000_1.lineInLevel(12);

    pinMode(A10, INPUT);

    tuner.begin(0.3);
    tft.begin();
    tft.fillScreen(ILI9488_BLACK);
    tft.setRotation(3);

    displayText("Hello");
}

elapsedMillis volmsec = 0;

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
            delay_effect.next_param(param_selector_new - param_selector_prev);
            param_selector_prev = param_selector_new;
        }

        int value_selector_new = value_selector.read() / 4;
        if (value_selector_new != value_selector_prev)
        {
            delay_effect.change_param(value_selector_new - value_selector_prev);
            value_selector_prev = value_selector_new;
        }

        // reverb_ms = 100 + encoder_value_selector.read() / 4 * 20;

        // if (reverb_ms != prev_reverb_ms)
        // {
        //   Serial.println("Reverb: " + String(reverb_ms));
        //   reverb1.reverbTime(reverb_ms / 1000.0);
        //   prev_reverb_ms = reverb_ms;
        // }

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
}