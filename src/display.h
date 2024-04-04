#pragma once
#include "ILI9488_t3.h"

#define TFT_RST 34
#define TFT_DC 9
#define TFT_CS 36
#define MOSI 11
#define MISO 12
#define SCLK 13
#define T_CS 33

extern ILI9488_t3 tft;

void displayText(const String &s);