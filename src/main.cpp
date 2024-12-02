#include <Arduino.h>
#include <Encoder.h>
#include <Audio.h>
#include "SPI.h"
#include <TFT_eSPI.h> 
#include "OneButton.h"

#include <effects/delay.cpp>
#include <effects/reverb.cpp>
#include <effects/tremolo.cpp>
#include <effects/chorus.cpp>
#include <effects/flanger.cpp>
#include <interface/display.h>
#include <lvgl.h>
#define TFT_RGB_ORDER TFT_BGR
#include <presets/presets.h>
#include "interface/led_driver.h"
#include <interface/button_decoder.h>
#include "interface/tuner.h"

#define MASTER_POT A1
#define HP_VOLUME_POT A2
#define EXP A3
#define BACKLIGHT_PWM 37
#define SW_BYPASS 0
#define PWM_FREQ 100000

#define MAX_VOLUME 0.7

#define CHAIN_LENGTH 5
Effect *effects_chain[] = {new Chorus(), new Tremolo(), new Flanger(), new Delay(), new Reverb()};
Effect *chorus_effect = effects_chain[0];
Effect *tremolo_effect = effects_chain[1];
Effect *delay_effect = effects_chain[2];
Effect *reverb_effect = effects_chain[3];

#define DRY_CHANNEL 0
#define WET_CHANNEL 1

int selected_effect_index = 0;
// GUItool: begin automatically generated code
AudioInputI2S audio_input;       // xy=334.1037826538086,172.99997901916504
AudioMixer4 final_mixer;       // xy=681.1037902832031,179
AudioAnalyzeNoteFrequency tuner; // xy=1014.1037216186523,270.00000953674316
AudioAnalyzeRMS rms_meter;
AudioOutputI2S audio_output;     // xy=1036.1037902832031,219

std::vector<AudioConnection*> patch_cords(6 + CHAIN_LENGTH * 4);

void create_audio_connections() {
    // Connect input to the start of the effects chain
    patch_cords.push_back(new AudioConnection(audio_input, effects_chain[0]->input_amp));
    patch_cords.push_back(new AudioConnection(audio_input, 0, final_mixer, DRY_CHANNEL));

    // Connect effects together
    for (size_t i = 0; i < CHAIN_LENGTH; i++) {
        patch_cords.push_back(new AudioConnection(effects_chain[i]->input_amp, 0, effects_chain[i]->dry_wet_mixer, DRY_CHANNEL)); // Dry to dry_wet_mixer
        patch_cords.push_back(new AudioConnection(effects_chain[i]->input_amp, *effects_chain[i]->chain_start));  // Wet in
        patch_cords.push_back(new AudioConnection(*effects_chain[i]->chain_end, 0, effects_chain[i]->dry_wet_mixer, WET_CHANNEL)); // Wet out to dry_wet_mixer

        if (i < CHAIN_LENGTH - 1) {
            patch_cords.push_back(new AudioConnection(effects_chain[i]->dry_wet_mixer, effects_chain[i + 1]->input_amp));
        }
    }

    // Connect last effect to output
    patch_cords.push_back(new AudioConnection(effects_chain[CHAIN_LENGTH - 1]->dry_wet_mixer, 0, final_mixer, WET_CHANNEL));
    patch_cords.push_back(new AudioConnection(final_mixer, 0, audio_output, 0));
    patch_cords.push_back(new AudioConnection(final_mixer, 0, audio_output, 1));

    // Additional connections
    patch_cords.push_back(new AudioConnection(audio_input, tuner));
    patch_cords.push_back(new AudioConnection(audio_input, rms_meter));
}


AudioControlSGTL5000 sgtl5000_1; // xy=256.1037902832031,460


// OneButton button_1(3, true);
// Encoder encoder_1(4, 5);

OneButton button_2(25, true, true);
Encoder encoder_2(29, 28);

OneButton button_3(30, true, true);
Encoder encoder_3(32, 31);


extern TFT_eSPI tft;


elapsedMillis volmsec = 0;

std::vector<AudioConnection> patchCords;

extern preset_bank_t preset_bank;

void setup()
{
    analogWriteFrequency(BACKLIGHT_PWM, PWM_FREQ);
    analogWriteFrequency(LED_PWM, PWM_FREQ);

    setup_leds();
    

    // analogWrite(BACKLIGHT_PWM, 0); //Initalize display pwm early to avoid flicker
    Serial.begin( 115200 );
    // setup_leds(); // TODO: blink all leds on startup
    // Audio connections require memory to work.  For more
    // detailed information, see the MemoryAndCpuUsage example
    AudioMemory(1200);

	//TODO: toggle dry/wet for entire chain
    final_mixer.gain(0, 0);
    final_mixer.gain(1, 1);

	create_audio_connections();

    sgtl5000_1.lineInLevel(0);
    sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
    sgtl5000_1.adcHighPassFilterDisable();
    sgtl5000_1.volume(0.5);
    sgtl5000_1.enable();

    pinMode(MASTER_POT, INPUT);
    pinMode(HP_VOLUME_POT, INPUT);
    pinMode(EXP, INPUT);
    pinMode(SW_BYPASS, INPUT_PULLUP);

    pinMode(BACKLIGHT_PWM, OUTPUT);
    tuner.begin(TUNER_THRESHOLD);
    while(!Serial && millis() < 2500){};

	if(Serial)
		Serial.printf("Serial connected in %d\n", millis());

	init_display();
    create_status_bar();
	create_effect_lists(effects_chain, CHAIN_LENGTH);

    //Initialize preset bank
    preset_bank = {0, NUM_PRESETS, CHAIN_LENGTH, {}};

    //Load active preset
    //TODO: save active preset when it is selected, not just when overwritten
	LV_LOG_USER("Loading saved presets...");
	load_presets_from_eeprom(&preset_bank);
	LV_LOG_USER("Applying active preset...");
	load_preset(preset_get_active_index());

	digitalWrite(BACKLIGHT_PWM, 1);
	
	setup_decoder();

    led_set(255, false);

}

void loop()
{
    static bool blink_state = 0;
    static float prev_volume = 0.0;
    static float prev_mix = 0.0;

    if (volmsec > 350)
    {
        // led_toggle(255 - LED_STATUS);
        volmsec = 0;  


        // Serial.println(mix);
		// led_toggle(LED_STATUS);
        // led_set(LED_BYPASS, !digitalRead(SW_BYPASS));

        #ifdef LOG_RAM_USAGE
            Serial.printf("CPU: %f, %f\nMemory: %d, %d\n", AudioProcessorUsage(), AudioProcessorUsageMax(), AudioMemoryUsage(), AudioMemoryUsageMax());
        #endif

        blink_state = !blink_state;
        digitalWrite(LED_BUILTIN, blink_state);
    }

    // statusbar_log_fmt("Mix %.2f HP: %.2f", 1- mix, volume);

    float volume = analogRead(HP_VOLUME_POT);
    volume = volume / 1023.0;

    if(fabs(volume - prev_volume) > 0.05)
    {
        sgtl5000_1.volume(volume * MAX_VOLUME);
        statusbar_set_headphone_arc(volume);

        prev_volume = volume;
    }


    float mix = analogRead(MASTER_POT) / 1023.0;
    if(fabs(mix - prev_mix) > 0.05)
    {
        final_mixer.gain(DRY_CHANNEL, mix);
        final_mixer.gain(WET_CHANNEL, 1 - mix);

        statusbar_set_mix_arc(1 - mix);

        prev_mix = mix;
    }

    // button_1.tick();
    button_2.tick();
    button_3.tick();

	lv_timer_handler();
    decoder_tick();

    if(rms_meter.available())
    {
        float rms = rms_meter.read();
        // statusbar_log_fmt("RMS: %.3f", rms_meter.read());

        led_set(LED_STATUS, rms > 0.1f);
    }
    else led_set(LED_STATUS, 0);

    tuner_tick();

    delay(25);
}