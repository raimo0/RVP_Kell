#include <Arduino.h>
#include "leds.h"

int praeguneVarv = 0;
int valitudColorPreset = 1;
int valitudBrightness = 100;
char colors[4][7] = {"000000", "00ff00", "ff0000", "0000ff"};
CRGB leds[NUM_LEDS];

void ledSetup()
{
  pinMode(LEDH, OUTPUT);
  pinMode(LEDM, OUTPUT);
  pinMode(LED_DATA_PIN, OUTPUT);
  pinMode(debugLED, OUTPUT);
  digitalWrite(LEDH, HIGH);
  digitalWrite(LEDM, HIGH);
  // LED riba init
  FastLED.addLeds<WS2812, LED_DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(valitudBrightness); // Ledide brightness
}

void ledUpdate()
{
  char r[3];
  r[0] = colors[valitudColorPreset][0];
  r[1] = colors[valitudColorPreset][1];
  char g[3];
  g[0] = colors[valitudColorPreset][2];
  g[1] = colors[valitudColorPreset][3];
  char b[3];
  b[0] = colors[valitudColorPreset][4];
  b[1] = colors[valitudColorPreset][5];
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(strtol(r, NULL, 16), strtol(g, NULL, 16), strtol(b, NULL, 16));
  }
  FastLED.show();
}