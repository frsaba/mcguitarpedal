#include "display.h"

#define NUM_SETTINGS 4
#define MAX_BRIGHTNESS 255
#define BRIGHTNESS_STEPS 5
#define THRESHOLD_STEPS 0.01

extern AudioAnalyzeNoteFrequency tuner; 
extern float tuner_threshold;
extern AudioAnalyzeRMS rms_meter;
extern float rms_threshold;

FLASHMEM void settings_setup();
void settings_hide();
void settings_show();