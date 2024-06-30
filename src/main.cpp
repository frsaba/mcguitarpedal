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
#include <TFT_eSPI.h> 
#include <SD.h>
#include <SerialFlash.h>
#include "OneButton.h"

#include <effects/delay.cpp>
#include <effects/reverb.cpp>
#include <effects/tremolo.cpp>
#include <effects/chorus.cpp>
#include <display.h>
#include <lvgl.h>
#define TFT_RGB_ORDER TFT_BGR

#define CHAIN_LENGTH 4
Effect *effects_chain[] = {new Chorus(), new Tremolo(), new Delay(), new Reverb()};
Effect *chorus_effect = effects_chain[0];
Effect *tremolo_effect = effects_chain[1];
Effect *delay_effect = effects_chain[2];
Effect *reverb_effect = effects_chain[3];

int selected_effect_index = 0;
// GUItool: begin automatically generated code
AudioInputI2S audio_input;       // xy=334.1037826538086,172.99997901916504
AudioMixer4 dry_wet_mixer;       // xy=681.1037902832031,179
AudioAnalyzeNoteFrequency tuner; // xy=1014.1037216186523,270.00000953674316
AudioOutputI2S audio_output;     // xy=1036.1037902832031,219

// Create connections manually
AudioConnection patchCord1(audio_input, 0, chorus_effect->input_amp, 0);

AudioConnection patchCord17(chorus_effect->input_amp, 0, chorus_effect->dry_wet_mixer, 0); // Dry to dry_wet_mixer
AudioConnection patchCord18(chorus_effect->input_amp, 0, *chorus_effect->chain_start, 0);  // Wet in
AudioConnection patchCord19(*chorus_effect->chain_end, 0, chorus_effect->dry_wet_mixer, 1); // Wet out to dry_wet_mixer

AudioConnection patchCord20(chorus_effect->dry_wet_mixer, 0, tremolo_effect->input_amp, 0);

AudioConnection patchCord16(tremolo_effect->input_amp, 0, tremolo_effect->dry_wet_mixer, 0); // Dry to dry_wet_mixer
AudioConnection patchCord13(tremolo_effect->input_amp, 0, *tremolo_effect->chain_start, 0);  // Wet in
AudioConnection patchCord14(*tremolo_effect->chain_end, 0, tremolo_effect->dry_wet_mixer, 1); // Wet out to dry_wet_mixer

AudioConnection patchCord15(tremolo_effect->dry_wet_mixer, 0, delay_effect->input_amp, 0);

AudioConnection patchCord2(delay_effect->input_amp, 0, delay_effect->dry_wet_mixer, 0); // Dry to dry_wet_mixer
AudioConnection patchCord3(delay_effect->input_amp, 0, *delay_effect->chain_start, 0);  // Wet in
AudioConnection patchCord4(*delay_effect->chain_end, 0, delay_effect->dry_wet_mixer, 1); // Wet out to dry_wet_mixer

AudioConnection patchCord5(delay_effect->dry_wet_mixer, 0, reverb_effect->input_amp, 0);
AudioConnection patchCord6(reverb_effect->input_amp, 0, reverb_effect->dry_wet_mixer, 0); // Dry to dry_wet_mixer
AudioConnection patchCord7(reverb_effect->input_amp, 0, *reverb_effect->chain_start, 0);  // Wet in
AudioConnection patchCord8(*reverb_effect->chain_end, 0, reverb_effect->dry_wet_mixer, 1); // Wet out to dry_wet_mixer

AudioConnection patchCord9(reverb_effect->dry_wet_mixer, 0, dry_wet_mixer, 1); // Final mix to main dry/wet mixer

// Final output connections
AudioConnection patchCord10(dry_wet_mixer, 0, audio_output, 0);
AudioConnection patchCord11(dry_wet_mixer, 0, audio_output, 1);
AudioConnection patchCord12(dry_wet_mixer, tuner);

AudioControlSGTL5000 sgtl5000_1; // xy=256.1037902832031,460
// GUItool: end automatically generated code

// TODO: feedback delay?

const int myInput = AUDIO_INPUT_LINEIN;
// const int myInput = AUDIO_INPUT_MIC;

Encoder encoder_1(27, 28);
int encoder_1_prev = 0;

Encoder encoder_2(31, 32);
int encoder_2_prev = 0;

Encoder encoder_3(29, 30);
int encoder_3_prev = 0;


extern TFT_eSPI tft;

OneButton button_1(37, true);
OneButton button_2(38, true);
OneButton button_3(39, true);

elapsedMillis volmsec = 0;

std::vector<AudioConnection> patchCords;

void setup()
{
	
    Serial.begin( 115200 );
    // Audio connections require memory to work.  For more
    // detailed information, see the MemoryAndCpuUsage example
    AudioMemory(1200);

	//TODO: toggle dry/wet for entire chain
    dry_wet_mixer.gain(0, 0.3);
    dry_wet_mixer.gain(1, 1);


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
    // button_1.attachClick([]()
    //                     {	
	// 						//TODO: Create macro for selected effect
    //                         selected_effect_index = (selected_effect_index + 1) % CHAIN_LENGTH;
    //                         displayText("Selected effect: " + String(effects_chain[selected_effect_index]->name)); });
    // button2.attachClick([]()
    //                     { 
    //                         effects_chain[selected_effect_index]->toggle_bypass(); 
    //                         });
    //TODO: long press to reset
	button_3.attachClick([]()
                        { displayText("Button 3 pressed"); });

    tuner.begin(0.3);
    // tft.begin();
    // tft.fillScreen(TFT_BLACK);
    // tft.setRotation(3);

    // displayText("Hello");

	init_display();
	create_effect_lists(effects_chain, CHAIN_LENGTH);
}

void loop()
{
    if (volmsec > 150)
    {
        float vol = analogRead(A10);
        vol = vol / 1023.0;
        sgtl5000_1.volume(vol); // <-- uncomment if you have the optional
        volmsec = 0;            //     volume pot on your audio shield

        int param_selector_new = encoder_2.read() / 4;
        if (param_selector_new != encoder_2_prev)
        {
            effects_chain[selected_effect_index]->next_param(param_selector_new - encoder_2_prev);
            encoder_2_prev = param_selector_new;
        }

        int value_selector_new = encoder_3.read() / 4;
        if (value_selector_new != encoder_3_prev)
        {
            effects_chain[selected_effect_index]->change_param(value_selector_new - encoder_3_prev);
            encoder_3_prev = value_selector_new;
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
    button_1.tick();
    button_2.tick();
    button_3.tick();
	lv_timer_handler();
    delay(10);
}