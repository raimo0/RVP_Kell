#ifndef LEDS_H
#define LEDS_H
#include <FastLED.h>


#define NUM_LEDS 11
#define LED_DATA_PIN 15

int praeguneVarv = 0;
extern int ledBrightness;
extern const int LEDH;
extern const int LEDM;
extern const int LED_riba;
extern const int debugLED;
const int varviValikud[] = {0, 32, 64, 96, 128, 160, 192, 224}; // Red, Orange, Yellow, Green, Aqua, Blue, Purple, Pink
int varviIndeks = 0;

void ledSetup();


#endif // LEDS_H
