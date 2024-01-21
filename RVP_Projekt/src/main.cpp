#include <LiquidCrystal.h>
#include <FastLED.h>
#include <WiFi.h>
#include <time.h>
#include <Arduino.h>
#include "motors.h"
#include <WiFiManager.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ESPmDNS.h>
#include <string>
#include<iostream>
// Connectors
#define LEDH 13
#define LEDM 12
#define VALGUS_SENSOR 35
#define NUPP 4
#define LED_RIBA 15
#define NUM_LEDS 11

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

int previousMinute = -1;
int prev_button_state = 1;

int selected_color_preset = 1;
int selected_brightness = 100;
char colors[3][7] = {"ff0000", "00ff00", "0000ff"};

AsyncWebServer server(80); 
AsyncWebSocket webSocket("/ws");
char msg_buf[30];

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *payload, size_t len){
  switch(type) {
    case WS_EVT_DISCONNECT:
      Serial.printf("[%u] Disconnected!\n", 4);
      break;
    case WS_EVT_CONNECT:
      {
        IPAddress ip = client->remoteIP();
        Serial.printf("[%u] Connection from ", client->id());
        Serial.println(ip.toString());
        for (int i = 0; i < 3; i++)
        {
          sprintf(msg_buf, "color%i:#%s", i+1, colors[i]);
          Serial.printf("Sending to [%s]: %s\n", ip.toString(), msg_buf);
          client->text(msg_buf);
        }
        sprintf(msg_buf, "selectedcolor:%i", selected_color_preset);
        Serial.printf("Sending to [%s]: %s\n", ip.toString(), msg_buf);
        client->text(msg_buf);
        sprintf(msg_buf, "brightness:%i", selected_brightness);
        Serial.printf("Sending to [%s]: %s\n", ip.toString(), msg_buf);
        client->text(msg_buf);
      }
      break;
    case WS_EVT_DATA:
      Serial.printf("[%s] Received text: %s\n", client->remoteIP().toString(), payload);
      if ( strncmp((char *)payload, "color", 5) == 0 ) {
        char *token1 = strtok((char *)payload, "#");
        char token2[7];
        strncpy(token2, strtok(NULL, "#"), 6);
        if(strcmp(token1, "color1") == 0) sprintf(colors[0], "%s", token2);
        else if(strcmp(token1, "color2") == 0) sprintf(colors[1], "%s", token2);
        else if(strcmp(token1, "color3") == 0) sprintf(colors[2], "%s", token2);
        sprintf(msg_buf, "%s:#%s", token1, token2);
        Serial.printf("Sending: %s\n", msg_buf);
        webSocket.textAll(msg_buf);
      } else if ( strncmp((char *)payload, "selectedcolor", 13) == 0 ) {
        char *token1 = strtok((char *)payload, "#");
        char token2[7];
        strncpy(token2, strtok(NULL, "#"), 6);
        selected_color_preset = atoi(token2);
        sprintf(msg_buf, "%s:%s", token1, token2);
        Serial.printf("Sending: %s\n", msg_buf);
        webSocket.textAll(msg_buf);
      } else if ( strncmp((char *)payload, "brightness", 10) == 0 ) {
        char *token1 = strtok((char *)payload, "#");
        char token2[7];
        strncpy(token2, strtok(NULL, "#"), 6);
        selected_brightness = atoi(token2);
        FastLED.setBrightness(selected_brightness);
        sprintf(msg_buf, "%s:%s", token1, token2);
        Serial.printf("Sending: %s\n", msg_buf);
        webSocket.textAll(msg_buf);
      } else {
        Serial.printf("[%u] Message not recognized\n", 9);
      }
      break;
    default:
      break;
  }
}

void setup(){
  Serial.begin(115200);
  motorSetup();
  WiFiManager wm;
  //wm.resetSettings();

  if( !SPIFFS.begin()){
    Serial.println("Error mounting SPIFFS");
    while(1);
  }

  pinMode(LEDH, OUTPUT);
  pinMode(LEDM, OUTPUT);
  pinMode(LED_RIBA, OUTPUT);
  pinMode(NUPP, INPUT);
  pinMode(VALGUS_SENSOR, INPUT);
  pinMode(debugLED, OUTPUT);
  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

  if(!res) {
      // Ei saanud ühendust
      Serial.println("Failed to connect");
      // ESP.restart();
  } 
  else {
      // Sai ühenduse 
      Serial.println("connected...yeey :)");
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

      //Kella seadetele saab ligi http://rvpkell.local
      if (!MDNS.begin("rvpkell")) {
      Serial.println("Error MDNS!");
      while(1) {
          delay(1000);
      }
      }
  }
  digitalWrite(LEDH, HIGH); // Pin High

  // LED riba init
  FastLED.addLeds<WS2812, LED_RIBA, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(selected_brightness); //Ledide brightness

  // LCD
  lcd.begin(16, 2);
  lcd.clear();
  digitalWrite(LEDH,HIGH);

  // Veebiserver
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("favicon");
    request->send(SPIFFS, "/favicon.png", "image/png");
  });
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });
  webSocket.onEvent(onWsEvent);
  server.addHandler(&webSocket);
  server.begin();
}

void loop(){
  // Motor control
  
  plaadiLiigutamine();
  liigutaMinutiMootor("vasak",200);

  // LED riba
  /*int ledBrightness = 100;
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
  FastLED.show();*/

  //Nupp
  if(digitalRead(NUPP) == LOW && prev_button_state == HIGH) {
    selected_color_preset = (selected_color_preset + 1) % 3;
  }
  prev_button_state = digitalRead(NUPP);

  //Led riba
  int color_num = (int)strtol(colors[selected_color_preset-1], NULL, 16);
  for (int i = 0; i < 11; i++)
  {
    leds[i] = CRGB((color_num & 0xff0000) >> 4, (color_num & 0x00ff00) >> 2, (color_num & 0x0000ff));
  }
  
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
