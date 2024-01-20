#include <LiquidCrystal.h>
#include <FastLED.h>
#include <WiFi.h>
//#include <ESPAsyncWebServer.h>
#include <time.h>
#include <Arduino.h>
#include "motors.h"
#include <WiFiManager.h>

#define NUM_LEDS 30
#define LED_DATA_PIN 15

// Led riba
CRGB leds[NUM_LEDS];

// LCD
LiquidCrystal lcd(21, 2, 19, 18, 5, 17, 16);

// Variables

int debugLED = 22;
const char *ssid = "Hotspot";
const char *password = "123456789";
const char *ntpServer = "0.europe.pool.ntp.org";
const long gmtOffset_sec = 7200;
const int daylightOffset_sec = 3600;

// Connectors
int LEDH = 13;
int LEDM = 12;
int valgusSensor = 35;
int nupp = 4;
int LED_riba = 15;
int previousMinute = -1;
//AsyncWebServer server(80);
// put function declarations here:



void setup(){
  Serial.begin(115200);
  motorSetup();
  WiFiManager wm;
  pinMode(LEDH,OUTPUT);
  pinMode(LEDM,OUTPUT);
  pinMode(LED_riba,OUTPUT);
  pinMode(nupp,INPUT);
  pinMode(valgusSensor,INPUT);
  pinMode(debugLED, OUTPUT);
  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

  if(!res) {
      Serial.println("Failed to connect");
      // ESP.restart();
  } 
  else {
      //if you get here you have connected to the WiFi    
      Serial.println("connected...yeey :)");
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  }
  digitalWrite(LEDH, HIGH); // Pin High

  // LED riba
  FastLED.addLeds<WS2812, LED_DATA_PIN, RGB>(leds, NUM_LEDS);

  // LCD
  lcd.begin(16, 2);
  lcd.clear();
  digitalWrite(LEDH,HIGH);

}

void loop(){
  // Motor control
  
  plaadiLiigutamine();
  liigutaMinutiMootor("vasak",200);

  // LED riba
  int ledBrightness = 100;
  leds[0] = CHSV(0,255,ledBrightness);
  leds[1] = CHSV(32,255,ledBrightness);
  leds[2] = CHSV(96,255,ledBrightness);
  leds[3] = CHSV(24,255,ledBrightness);
  leds[4] = CHSV(0,255,ledBrightness);
  leds[5] = CHSV(32,255,ledBrightness);
  leds[6] = CHSV(96,255,ledBrightness);
  leds[7] = CHSV(24,255,ledBrightness);
  leds[8] = CHSV(0,255,ledBrightness);
  leds[9] = CHSV(32,255,ledBrightness);
  leds[10] = CHSV(96,255,ledBrightness);
  FastLED.show();
  
  
  time_t now = time(nullptr);
  struct tm *timeInfo;
  timeInfo = localtime(&now);
  lcd.setCursor(0, 1);
  lcd.print("Kell: ");
  lcd.print(timeInfo->tm_hour);
  lcd.print(":");
  lcd.print(timeInfo->tm_min);
  lcd.print(":");
  lcd.print(timeInfo->tm_sec);
  
  if (timeInfo->tm_min != previousMinute) {
    if (previousMinute == 59) {
      liigutaMinutiMootor("vasak",1200);
    }
    liigutaMinutiMootor("vasak",200);
    previousMinute = timeInfo->tm_min;
  }
  lcd.setCursor(0,0);
  lcd.print(previousMinute);
  delay(100);
  
}
