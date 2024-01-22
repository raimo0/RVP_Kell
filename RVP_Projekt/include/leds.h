#ifndef LEDS_H
#define LEDS_H
#include <FastLED.h>


#define NUM_LEDS 11
#define LED_DATA_PIN 15
#define LEDH 13
#define LEDM 12

int praeguneVarv = 0;
int selected_color_preset = 1;
int selected_brightness = 100;

extern int ledBrightness;

void ledSetup();


#endif // LEDS_H