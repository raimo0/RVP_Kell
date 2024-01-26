#include <LiquidCrystal.h>
#include <FastLED.h>
#include <WiFi.h>
#include <time.h>
#include <Arduino.h>
#include "motors.h"
#include "leds.h"
#include <WiFiManager.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ESPmDNS.h>

// Connectors
#define NUPP 4

// Led riba
CRGB leds[NUM_LEDS];

// LCD
LiquidCrystal lcd(21, 2, 19, 18, 5, 17, 16);

// Variables
const char *ntpServer = "0.europe.pool.ntp.org";
const long gmtOffset_sec = 7200;
const int daylightOffset_sec = 3600;
unsigned long button_time = 0;
unsigned long last_button_time = 0;
int eelmineTund = 0;
int eelmineMinut = 0;
int totalSteps = 0;
char colors[4][7] = {"000000", "00ff00", "ff0000", "0000ff"};
bool kas_nupuvajutus = false;

AsyncWebServer server(80);
AsyncWebSocket webSocket("/ws");
char msg_buf[30];

void IRAM_ATTR isr()
{
  kas_nupuvajutus = true;
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
    selected_color_preset++;
    if (selected_color_preset > 3)
      selected_color_preset = 0;
    last_button_time = button_time;
  }
}


void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *payload, size_t len)
{
  // Kui toimus nupuvajutus, siis saada info edasi
  if (kas_nupuvajutus)
  {
    sprintf(msg_buf, "selectedcolor:%i", selected_color_preset);
    Serial.println(msg_buf);
    webSocket.textAll(msg_buf);
    kas_nupuvajutus = false;
  }

  switch (type)
  {
  case WS_EVT_DISCONNECT:
    Serial.printf("[%u] Disconnected!\n", 4);
    break;
  case WS_EVT_CONNECT:
  {
    IPAddress ip = client->remoteIP();
    Serial.printf("[%u] Connection from ", client->id());
    Serial.println(ip.toString());
    for (int i = 1; i <= 3; i++)
    {
      sprintf(msg_buf, "color%i:#%s", i, colors[i]);
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
    if (strncmp((char *)payload, "color", 5) == 0)
    {
      char *token1 = strtok((char *)payload, "#");
      char token2[7];
      strncpy(token2, strtok(NULL, "#"), 6);
      token2[6] = 0;
      if (strcmp(token1, "color1") == 0)
        sprintf(colors[1], "%s", token2);
      else if (strcmp(token1, "color2") == 0)
        sprintf(colors[2], "%s", token2);
      else if (strcmp(token1, "color3") == 0)
        sprintf(colors[3], "%s", token2);
      sprintf(msg_buf, "%s:#%s", token1, token2);
      Serial.printf("Sending: %s\n", msg_buf);
      webSocket.textAll(msg_buf);
    }
    else if (strncmp((char *)payload, "selectedcolor", 13) == 0)
    {
      char *token1 = strtok((char *)payload, "#");
      char *token2 = strtok(NULL, "#");
      int digits = 0;
      for (int i = 0; i < 4; i++)
      {
        if (isdigit(token2[i]))
          digits++;
      }
      token2[digits] = 0;
      selected_color_preset = atoi(token2);
      sprintf(msg_buf, "%s:%s", token1, token2);
      Serial.printf("Sending: %s\n", msg_buf);
      webSocket.textAll(msg_buf);
    }
    else if (strncmp((char *)payload, "brightness", 10) == 0)
    {
      char *token1 = strtok((char *)payload, "#");
      char *token2 = strtok(NULL, "#");
      int digits = 0;
      for (int i = 0; i < 4; i++)
      {
        if (isdigit(token2[i]))
          digits++;
      }
      token2[digits] = 0;
      strncpy(token2, token2, digits);
      selected_brightness = atoi(token2);
      FastLED.setBrightness(selected_brightness);
      sprintf(msg_buf, "%s:%s", token1, token2);
      Serial.printf("Sending: %s\n", msg_buf);
      webSocket.textAll(msg_buf);
    }
    else
    {
      Serial.printf("[%u] Message not recognized\n", 9);
    }
    break;
  default:
    break;
  }
}

void setup()
{
  Serial.begin(115200);
  // LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Seadistus");
  motorSetup();
  ledSetup();
  mootoridAlgusesse();
  // plaadiLiigutamine("parem");
  //tundStarti();
  //minutStarti();
  //liigutaMinutiMootor(-3500);
  lcd.clear();
  WiFiManager wm;
   //wm.resetSettings();
  if (!SPIFFS.begin())
  {
    Serial.println("Error mounting SPIFFS");
    while (1)
      ;
  }

  pinMode(NUPP, INPUT);
  bool res;
  res = wm.autoConnect("Seadistus"); // password protected ap

  if (!res)
  {
    Serial.println("Ülesseadmine ebaõnnestus. Palun sisesta WiFi andmed");
    lcd.setCursor(0, 1);
    lcd.print("Seadista WiFi");
    // ESP.restart();
  }
  else
  {
    Serial.println("Ühendus loodud!");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    delay(4000);
    // Veebiserver
    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/favicon.png", "image/png"); });
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/index.html", "text/html"); });
    webSocket.onEvent(onWsEvent);
    server.addHandler(&webSocket);
    server.begin();
    // Kella seadetele saab ligi http://rvpkell.local
    if (!MDNS.begin("rvpkell"))
    {
      Serial.println("Error MDNS!");
      while (1)
      {
        delay(1000);
      }
    }
  }
  attachInterrupt(NUPP, isr, FALLING);

  // LED riba init
  FastLED.addLeds<WS2812, LED_DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(selected_brightness); // Ledide brightness



}

void loop()
{

  // Led riba
  char r[3];
  r[0] = colors[selected_color_preset][0];
  r[1] = colors[selected_color_preset][1];
  char g[3];
  g[0] = colors[selected_color_preset][2];
  g[1] = colors[selected_color_preset][3];
  char b[3];
  b[0] = colors[selected_color_preset][4];
  b[1] = colors[selected_color_preset][5];
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB(strtol(r, NULL, 16), strtol(g, NULL, 16), strtol(b, NULL, 16));
  }
  FastLED.show();
  time_t now = time(nullptr);
  struct tm *timeInfo;
  timeInfo = localtime(&now);
  int currentHour = timeInfo->tm_hour;
  int currentMinute = timeInfo->tm_min;
  // LCD Display
  
  lcd.setCursor(0, 0);
  lcd.print("Kuup");
  lcd.print((char)0b10000100);
  lcd.setCursor(5, 0);
  lcd.print("ev: ");
  lcd.print(timeInfo->tm_mday);
  lcd.print("-");
  lcd.print(timeInfo->tm_mon + 1);
  lcd.print("-");
  lcd.print(timeInfo->tm_year % 100);
  lcd.setCursor(0,1);
  lcd.print(WiFi.SSID());

  /*
  lcd.setCursor(0, 0);
  lcd.print("Kell: ");
  lcd.print(timeInfo->tm_hour);
  lcd.print("-");
  lcd.print(timeInfo->tm_min);
  */
  if (Serial.available() > 0)
  {
    String input = Serial.readStringUntil('\n');
    Serial.println(input);
    int steps = input.toInt();
    kuvaTund(steps,5,5);
  }
  
  if (currentHour != eelmineTund)
  {
    if (currentHour >= 12)
    {
      plaadiLiigutamine("vasak",1000);
    }
    else if (currentHour < 12)
    {
      plaadiLiigutamine("parem",1000);
    }
  }

  if (currentMinute != eelmineMinut)
  {
    kuvaMinut(currentMinute);
    kuvaTund(currentHour, currentMinute, eelmineTund);
    eelmineMinut = currentMinute;
    eelmineTund = currentHour;
  }
  
}
