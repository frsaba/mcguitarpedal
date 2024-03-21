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
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s2;           //xy=382.1999816894531,238
AudioEffectDelay         delay1;         //xy=523.1999816894531,392
AudioAmplifier           amp1;           //xy=532.5999908447266,258.19998359680176
AudioEffectReverb        reverb1;        //xy=660.6000213623047,258.1999683380127
AudioMixer4              mixer1;         //xy=686.1999359130859,356.9999885559082
AudioMixer4              mixer2;         //xy=770.6000213623047,154.19998264312744
AudioAnalyzeRMS          rms1;           //xy=961.5999908447266,327.1999969482422
AudioFilterBiquad        biquad1;        //xy=967.6000289916992,251.1999979019165
AudioAnalyzeNoteFrequency notefreq1;      //xy=990.5999908447266,385.1999969482422
AudioAnalyzeNoteFrequency notefreq2; //xy=1117.6000270843506,301.20000171661377
AudioOutputI2S           i2s1;           //xy=1125.200065612793,194.99998664855957
AudioConnection          patchCord1(i2s2, 0, mixer2, 0);
AudioConnection          patchCord2(i2s2, 0, amp1, 0);
AudioConnection          patchCord3(i2s2, 1, delay1, 0);
AudioConnection          patchCord4(delay1, 0, mixer1, 0);
AudioConnection          patchCord5(delay1, 1, mixer1, 1);
AudioConnection          patchCord6(delay1, 2, mixer1, 2);
AudioConnection          patchCord7(delay1, 3, mixer1, 3);
AudioConnection          patchCord8(amp1, reverb1);
AudioConnection          patchCord9(reverb1, 0, mixer2, 1);
AudioConnection          patchCord10(mixer1, 0, mixer2, 2);
AudioConnection          patchCord11(mixer2, biquad1);
AudioConnection          patchCord12(mixer2, rms1);
AudioConnection          patchCord13(mixer2, notefreq1);
AudioConnection          patchCord14(mixer2, 0, i2s1, 0);
AudioConnection          patchCord15(biquad1, 0, i2s1, 1);
AudioConnection          patchCord16(biquad1, notefreq2);
AudioControlSGTL5000     sgtl5000_1;     //xy=345.1999816894531,435
// GUItool: end automatically generated code




//TODO: feedback delay?


const int myInput = AUDIO_INPUT_LINEIN;
// const int myInput = AUDIO_INPUT_MIC;
Encoder encoder1(31, 32);
int prev_delay_ms = 100;
int delay_ms = 100;

Encoder encoder2(29, 30);
int prev_reverb_ms = 100;
int reverb_ms = 100;

void setup()
{
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(500);

  // Enable the audio shield, select input, and enable output
  sgtl5000_1.enable();
  sgtl5000_1.inputSelect(myInput);
  sgtl5000_1.adcHighPassFilterDisable();
  sgtl5000_1.volume(0.5);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(A10, INPUT);

  mixer1.gain(0, 1);
  mixer1.gain(1, 0.75);
  mixer1.gain(2, 0.5);
  mixer1.gain(3, 0.25);

  amp1.gain(0.5);

  mixer2.gain(0, 1);   // Dry
  mixer2.gain(1, 0); // Reverb
  mixer2.gain(2, 0);   // Delay

  biquad1.setNotch(0, 1830, 400);
  biquad1.setNotch(1, 1830, 400);
  biquad1.setNotch(2, 1830, 400);
  notefreq1.begin(0.3);
  notefreq2.begin(0.1);
}

elapsedMillis volmsec = 0;

void loop()
{
  // every 50 ms, adjust the volume
  if (volmsec > 150)
  {
    float vol = analogRead(A10);
    vol = vol / 1023.0;
    sgtl5000_1.volume(vol); // <-- uncomment if you have the optional
    volmsec = 0;            //     volume pot on your audio shield

    delay_ms = 100 + encoder1.read() / 4 * 5;

    if (delay_ms != prev_delay_ms)
    {
      Serial.println("Delay: " + String(delay_ms));
      delay1.delay(0, delay_ms);
      delay1.delay(1, delay_ms * 2);
      delay1.delay(2, delay_ms * 3);
      delay1.delay(2, delay_ms * 4);
      prev_delay_ms = delay_ms;
    }

    reverb_ms = 100 + encoder2.read() / 4 * 20;

    if (reverb_ms != prev_reverb_ms)
    {
      Serial.println("Reverb: " + String(reverb_ms));
      reverb1.reverbTime(reverb_ms / 1000.0);
      prev_reverb_ms = reverb_ms;
    }

    if(notefreq1.available() && notefreq2.available()){
      notefreq2.read();
      Serial.println("Freq: " + String(notefreq1.read()) + " Probability: " + String(String(notefreq1.probability())) + " After filtering: " + String(notefreq2.probability()));
    }

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