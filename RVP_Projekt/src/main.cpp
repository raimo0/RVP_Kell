#include <LiquidCrystal.h>
#include <FastLED.h>
#include <WiFi.h>
#include <time.h>
#include <Arduino.h>
#include "motors.h"
#include "leds.h"
#include <WiFiManager.h>

#define NUM_LEDS 11
#define LED_DATA_PIN 15

// Led riba
CRGB leds[NUM_LEDS];

// LCD
LiquidCrystal lcd(21, 2, 19, 18, 5, 17, 16);

// Variables
const char *ssid = "Hotspot";
const char *password = "123456789";
const char *ntpServer = "0.europe.pool.ntp.org";
const long gmtOffset_sec = 7200;
const int daylightOffset_sec = 3600;

// Connectors
int valgusSensor = 35;
int nupp = 4;
int eelmineMinut = -1;
//AsyncWebServer server(80);
// put function declarations here:



void setup(){
  Serial.begin(115200);
  motorSetup();
  ledSetup();
  WiFiManager wm;
  pinMode(nupp,INPUT);
  pinMode(valgusSensor,INPUT);
  bool res;

  res = wm.autoConnect("Seadistus"); // password protected ap

  if(!res) {
    Serial.println("Ülesseadmine ebaõnnestus. Palun sisesta WiFi andmed");
    wm.startConfigPortal("Seadistus"); 
  } 
  else {
      Serial.println("Ühendus loodud!");
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  }

  // LED riba
  FastLED.addLeds<WS2812, LED_DATA_PIN, RGB>(leds, NUM_LEDS);

  // LCD
  lcd.begin(16, 2);
  lcd.clear();

}

void loop(){
  // Motor control
  
  plaadiLiigutamine();
  liigutaMinutiMootor("vasak",200);

  // LED riba
  for (int i = 0; i < NUM_LEDS; i++){
    leds[i] = CHSV(praeguneVarv,255,ledBrightness);
  }
  FastLED.show();
  
  
  time_t now = time(nullptr);

  struct tm *timeInfo;
  lcd.setCursor(0, 0);
  lcd.print("Kuupäev: ");
  lcd.print(timeInfo->tm_mday);   
  lcd.print("-");
  lcd.print(timeInfo->tm_mon + 1);    
  lcd.print("-");
  lcd.print(timeInfo->tm_year + 1900);
  
  
  timeInfo = localtime(&now);
  lcd.setCursor(0, 1);
  lcd.print("Kell: ");
  lcd.print(timeInfo->tm_hour);
  lcd.print(":");
  lcd.print(timeInfo->tm_min);
  lcd.print(":");
  lcd.print(timeInfo->tm_sec);
  
  if (timeInfo->tm_min != eelmineMinut) {
    if (eelmineMinut == 59) {
      liigutaMinutiMootor("vasak",1200);
    }
    liigutaMinutiMootor("vasak",200);
    eelmineMinut = timeInfo->tm_min;
  }
  if (timeInfo->tm_hour == 11 && timeInfo->tm_min == 59) {
    plaadiLiigutamine();
  } else if (timeInfo->tm_hour == 23 && timeInfo->tm_min == 59) {
    plaadiLiigutamine();
  }

  if (digitalRead(nupp) == LOW) {
    praeguneVarv = (praeguneVarv + 32) % 256; 
    delay(500); 
  }

  
}
