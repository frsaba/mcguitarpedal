#pragma once

#include "statusbar.h"

#define TUNER_THRESHOLD 0.05

#define REFERENCE_FREQ 440
#define MIDI_NOTE_OFFSET 69 //A4 in midi

#define CENTS_THRESHOLD 5.0
#define NUM_TUNER_LEDS 5
#define LED_SHIFT_SPEED_MS 50

extern const char *note_names[];

extern AudioAnalyzeNoteFrequency tuner;


void tuner_tick();