#include <interface/button_decoder.h>

void setup_decoder()
{
    pinMode(BUTTON_MTX_A, INPUT);
    pinMode(BUTTON_MTX_B, INPUT);
    pinMode(BUTTON_MTX_C, INPUT);
    pinMode(BUTTON_MTX_GS, INPUT);
}


void get_pressed_button()
{
    // Serial.printf("%d %d%d%d\n", digitalRead(BUTTON_MTX_GS), digitalRead(BUTTON_MTX_A), digitalRead(BUTTON_MTX_B), digitalRead(BUTTON_MTX_C));
    if(digitalRead(BUTTON_MTX_GS) == 0)
    {
        // Serial.printf("%d %d%d%d\n", digitalRead(BUTTON_MTX_GS), digitalRead(BUTTON_MTX_A), digitalRead(BUTTON_MTX_B), digitalRead(BUTTON_MTX_C));
        Serial.println(digitalRead(BUTTON_MTX_A) + (digitalRead(BUTTON_MTX_B) << 1) +  (digitalRead(BUTTON_MTX_C) << 2));

    }
}