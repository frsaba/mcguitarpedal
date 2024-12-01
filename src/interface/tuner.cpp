#include "tuner.h"

const char *note_names[12] = {
    "C",  "C#", "D",  "Eb", "E", 
    "F",  "F#", "G",  "G#", "A", 
    "Bb", "B"
};

void tuner_tick()
{
    if(!tuner.available())
    {
        led_set_tuner(0);
        // statusbar_log("No note");
        return;
    }

    float freq_hz = tuner.read();
    float semitones_from_a4 = 12 * log2(freq_hz/REFERENCE_FREQ) ;
    int nearest_note = roundf(semitones_from_a4);

    int note = (nearest_note + MIDI_NOTE_OFFSET) % 12;
    int cents = roundf((semitones_from_a4 - nearest_note) * 100);

    statusbar_log_fmt("%s: %d", note_names[note], cents);

    if(abs(cents) < CENTS_THRESHOLD)
    {
        led_set_tuner(255);
    }
    else
    {
        uint8_t led_index = constrain((cents/CENTS_THRESHOLD + NUM_TUNER_LEDS/2.0), 0, NUM_TUNER_LEDS - 1);
        led_set_tuner(1 << led_index);
    }    
}