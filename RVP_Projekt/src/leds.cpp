#include <Arduino.h>

#define NUM_LEDS 11
#define LED_DATA_PIN 15
#define LEDH 13
#define LEDM 12
#define debugLED 22
int ledBrightness = 100;


void ledSetup(){
  pinMode(LEDH,OUTPUT);
  pinMode(LEDM,OUTPUT);
  pinMode(LED_DATA_PIN,OUTPUT);
  pinMode(debugLED, OUTPUT);
  digitalWrite(LEDH,HIGH);
  digitalWrite(LEDM,HIGH);
}