#ifndef LEDS_H
#define LEDS_H
#include <FastLED.h>

#define NUM_LEDS 11
#define LED_DATA_PIN 15
#define LEDH 13
#define LEDM 12
#define debugLED 22

extern int praeguneVarv;
extern int valitudColorPreset;
extern int valitudBrightness;
extern char colors[4][7];

void ledSetup();
void ledUpdate();

#endif // LEDS_H