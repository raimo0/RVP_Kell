#include <LiquidCrystal.h>
// #include <FastLED.h>
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

// Mode list
#define AUTO 0
#define MANUAL 1
#define SAMMUD 2
int mode = 0;

// LCD
LiquidCrystal lcd(21, 2, 19, 18, 5, 17, 16);

// Variables
const char *ntpServer = "0.europe.pool.ntp.org";
const long gmtOffset_sec = 7200;
const int daylightOffset_sec = 3600;
unsigned long nupuAeg = 0;
unsigned long eelmineNupuAeg = 0;
int eelmineTund = 10000;
int eelmineMinut = 0;
bool kasNupuvajutus = false;

AsyncWebServer server(80);
AsyncWebSocket webSocket("/ws");
char buffer[30];

// Nupu interrupt
void IRAM_ATTR isr()
{
  kasNupuvajutus = true;
  nupuAeg = millis();
  if (nupuAeg - eelmineNupuAeg > 250)
  {
    valitudColorPreset++;
    if (valitudColorPreset > 3)
      valitudColorPreset = 0;
    eelmineNupuAeg = nupuAeg;
  }
}

// Websocket asjad
void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  // Kui toimus nupuvajutus, siis saada info edasi
  if (kasNupuvajutus)
  {
    sprintf(buffer, "selectedcolor:%i", valitudColorPreset);
    Serial.println(buffer);
    webSocket.textAll(buffer);
    kasNupuvajutus = false;
  }

  switch (type)
  {
  case WS_EVT_DISCONNECT:
    // Serial.printf("[%u] Disconnected!\n", 4);
    break;
  case WS_EVT_CONNECT:
  {
    IPAddress ip = client->remoteIP();
    // Serial.printf("[%u] Connection from ", client->id());
    // Serial.println(ip.toString());
    for (int i = 1; i <= 3; i++)
    {
      sprintf(buffer, "color%i:#%s", i, colors[i]);
      // Serial.printf("Sending to [%s]: %s\n", ip.toString(), buffer);
      client->text(buffer);
    }
    sprintf(buffer, "selectedcolor:%i", valitudColorPreset);
    // Serial.printf("Sending to [%s]: %s\n", ip.toString(), buffer);
    client->text(buffer);
    sprintf(buffer, "brightness:%i", valitudBrightness);
    // Serial.printf("Sending to [%s]: %s\n", ip.toString(), buffer);
    client->text(buffer);
  }
  break;
  case WS_EVT_DATA:
    // Serial.printf("[%s] Received text: %s\n", client->remoteIP().toString(), data);
    if (strncmp((char *)data, "color", 5) == 0)
    {
      char *token1 = strtok((char *)data, "#");
      char token2[7];
      strncpy(token2, strtok(NULL, "#"), 6);
      token2[6] = 0;
      if (strcmp(token1, "color1") == 0)
        sprintf(colors[1], "%s", token2);
      else if (strcmp(token1, "color2") == 0)
        sprintf(colors[2], "%s", token2);
      else if (strcmp(token1, "color3") == 0)
        sprintf(colors[3], "%s", token2);
      sprintf(buffer, "%s:#%s", token1, token2);
      // Serial.printf("Sending: %s\n", buffer);
      webSocket.textAll(buffer);
    }
    else if (strncmp((char *)data, "selectedcolor", 13) == 0)
    {
      char *token1 = strtok((char *)data, "#");
      char *token2 = strtok(NULL, "#");
      int digits = 0;
      for (int i = 0; i < 4; i++)
      {
        if (isdigit(token2[i]))
          digits++;
      }
      token2[digits] = 0;
      valitudColorPreset = atoi(token2);
      sprintf(buffer, "%s:%s", token1, token2);
      // Serial.printf("Sending: %s\n", buffer);
      webSocket.textAll(buffer);
    }
    else if (strncmp((char *)data, "brightness", 10) == 0)
    {
      char *token1 = strtok((char *)data, "#");
      char *token2 = strtok(NULL, "#");
      int digits = 0;
      for (int i = 0; i < 4; i++)
      {
        if (isdigit(token2[i]))
          digits++;
      }
      token2[digits] = 0;
      strncpy(token2, token2, digits);
      valitudBrightness = atoi(token2);
      FastLED.setBrightness(valitudBrightness);
      sprintf(buffer, "%s:%s", token1, token2);
      // Serial.printf("Sending: %s\n", buffer);
      webSocket.textAll(buffer);
    }
    else
    {
      // Serial.printf("[%u] Message not recognized\n", 9);
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

  // Seadistused
  motorSetup();
  ledSetup();
  mootoridAlgusesse();
  lcd.clear();

  // SPIFFS
  if (!SPIFFS.begin())
  {
    Serial.println("Error mounting SPIFFS");
    while (1)
      ;
  }

  // WifiManager
  WiFiManager wm;
  // wm.resetSettings();
  bool res;
  res = wm.autoConnect("Seadistus");
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
    lcd.setCursor(0,0);
    lcd.print("Kella uuendus");
    delay(4000);
    lcd.clear();
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

  // Nupp
  pinMode(NUPP, INPUT);
  attachInterrupt(NUPP, isr, FALLING);
}

void loop()
{
  ledUpdate();
  // Aeg
  time_t aeg = time(nullptr);
  struct tm *ajaInfo;
  ajaInfo = localtime(&aeg);
  int praeguneTund = ajaInfo->tm_hour;
  int praeguneMinut = ajaInfo->tm_min;
  // LCD Display
  lcd.setCursor(0, 0);
  lcd.print("Kuup");
  lcd.print((char)0b10000100); // ä
  lcd.setCursor(5, 0);
  lcd.print("ev: ");
  lcd.print(ajaInfo->tm_mday);
  lcd.print("-");
  lcd.print(ajaInfo->tm_mon + 1);
  lcd.print("-");
  lcd.print(ajaInfo->tm_year % 100);
  lcd.setCursor(0, 1);
  lcd.print(WiFi.SSID());
  // Mootori juhtimine serialist
  if (Serial.available() > 0)
  {
    String input = Serial.readStringUntil('\n');
    Serial.println(input);
    char *sisend = (char *)input.c_str();
    if (strncmp(sisend, "auto", 4) == 0)
    {
      mode = AUTO;
      kuvaMinut(praeguneMinut);
      kuvaTund(praeguneTund, praeguneMinut, eelmineTund);
      eelmineMinut = praeguneMinut;
      eelmineTund = praeguneTund;
    }
    else if (strncmp(sisend, "manual", 6) == 0)
    {
      mode = MANUAL;
    }
    else if (strncmp(sisend, "sammud", 6) == 0)
    {
      mode = SAMMUD;
    }
    if (mode == MANUAL)
    {
      if (strncmp(sisend, "kell", 4) == 0)
      {
        char *token = strtok(sisend, ":");
        char *tund = strtok(NULL, ":");
        char *minut = strtok(NULL, ":");
        kuvaTund(atoi(tund), atoi(minut), eelmineTund);
        kuvaMinut(atoi(minut));
        Serial.printf("%s:%s\n", tund, minut);
        eelmineTund = atoi(tund);
        eelmineMinut = atoi(minut);
      }
    }
    else if (mode == SAMMUD)
    {
      if (strncmp(sisend, "t", 1) == 0)
      {
        strtok(sisend, ":");
        int step = atoi(strtok(NULL, ":"));
        liigutaTunniMootor(step);
      }
      else if (strncmp(sisend, "m", 1) == 0)
      {
        strtok(sisend, ":");
        int step = atoi(strtok(NULL, ":"));
        liigutaMinutiMootor(step);
      }
    }
  }

  // Aja kuvamine
  if (mode == AUTO)
  {
    if (praeguneMinut != eelmineMinut)
    {
      kuvaMinut(praeguneMinut);
      kuvaTund(praeguneTund, praeguneMinut, eelmineTund);
      eelmineMinut = praeguneMinut;
      eelmineTund = praeguneTund;
    }
  }
}
