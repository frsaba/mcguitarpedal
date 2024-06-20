#include "display.h"

TFT_eSPI  tft = TFT_eSPI();

void displayText(const String &s)
{
    tft.fillRect(50, 150, 400, 60, TFT_BLACK);
    tft.setTextColor(TFT_GREENYELLOW);
    tft.setTextSize(2);
    tft.setCursor(50, 150);
    tft.print(s);
    Serial.println(s);
}