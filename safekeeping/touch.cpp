#include <Arduino.h>

#include "SPI.h"
#include "ILI9488_t3.h"

#define TFT_RST 34
#define TFT_DC 9
#define TFT_CS 10
#define MOSI 11
#define MISO 12
#define SCLK 13
#define T_CS 33
#define T_IRQ 35

ILI9488_t3 tft = ILI9488_t3(&SPI, TFT_CS, TFT_DC, TFT_RST, MOSI, SCLK, MISO);

#include <XPT2046_Touchscreen.h>
XPT2046_Touchscreen ts(T_CS, T_IRQ); 

void setup() {
  Serial.begin(38400);
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(ILI9488_BLACK);
  ts.begin();
  ts.setRotation(3);
  while (!Serial && (millis() <= 1000));
}

boolean wastouched = true;

void loop() {
  boolean istouched = ts.touched();
  if (istouched) {
    TS_Point p = ts.getPoint();
    if (!wastouched) {
      tft.fillScreen(ILI9488_BLACK);
      tft.setTextColor(ILI9488_YELLOW);
      tft.setTextSize(3);
      tft.setCursor(60, 80);
      tft.print("Touch");
    }
    tft.fillRect(100, 150, 140, 60, ILI9488_BLACK);
    tft.setTextColor(ILI9488_GREEN);
    tft.setTextSize(1);
    tft.setCursor(100, 150);
    tft.print("X = ");
    tft.print(p.x);
    tft.setCursor(100, 180);
    tft.print("Y = ");
    tft.print(p.y);
    Serial.print(", x = ");
    Serial.print(p.x);
    Serial.print(", y = ");
    Serial.println(p.y);
  } else {
    if (wastouched) {
      tft.fillScreen(ILI9488_BLACK);
      tft.setTextColor(ILI9488_RED);
      tft.setTextSize(2);
      tft.setCursor(120, 50);
      tft.print("No");
      tft.setCursor(80, 120);
      tft.print("Touch");
    }
    Serial.println("no touch");
  }
  wastouched = istouched;
  delay(100);
}