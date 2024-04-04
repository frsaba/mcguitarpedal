#include "display.h"

ILI9488_t3 tft = ILI9488_t3(&SPI, TFT_CS, TFT_DC, TFT_RST, MOSI, SCLK, MISO);

void displayText(const String &s)
{
    tft.fillRect(50, 150, 400, 60, ILI9488_BLACK);
    tft.setTextColor(ILI9488_GREEN);
    tft.setTextSize(2);
    tft.setCursor(50, 150);
    tft.print(s);
    Serial.println(s);
}