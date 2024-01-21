#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 11
#define LED_DATA_PIN 15

int praeguneVarv = 255;
int ledBrightness = 100;
const int LEDH = 13;
const int LEDM = 12;
const int LED_riba = 15;
const int debugLED = 22;



void ledSetup(){
  pinMode(LEDH,OUTPUT);
  pinMode(LEDM,OUTPUT);
  pinMode(LED_riba,OUTPUT);
  pinMode(debugLED, OUTPUT);
  digitalWrite(LEDH,HIGH);
  digitalWrite(LEDM,HIGH);
}