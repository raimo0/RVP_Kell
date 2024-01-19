#include <Arduino.h>
#include <LiquidCrystal.h>
#include <FastLED.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <time.h>



#define NUM_LEDS 30
#define LED_DATA_PIN 15

// Led riba
CRGB leds[NUM_LEDS];

// LCD
LiquidCrystal lcd(21, 2, 19, 18, 5, 17, 16);

// Mootorid
// Plaadi mootor
int directionNum = 33;
int stepNum= 32;
// Tunni mootor
int directionTund = 26;
int stepTund = 25;
// Minutite mootor
int directionMin = 14;
int stepMin = 27;

int numberOfSteps = 1200;
int kaugus = 4;
int nsleep = 23;

// Variables

int debugLED = 22;
const char *ssid = "Hotspot";
const char *password = "123456789";
const char *ntpServer = "0.europe.pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600;

// Connectors
int LEDH = 13;
int LEDM = 12;
int sensor = 35;
int nupp = 4;
int LED_riba = 15;

AsyncWebServer server(80);
// put function declarations here:

void moveStepper(int, int, int, int);

void setup()
{
  Serial.begin(115200);
  pinMode(LEDH,OUTPUT);
  pinMode(LEDM,OUTPUT);
  pinMode(LED_riba,OUTPUT);
  pinMode(nupp,INPUT);
  pinMode(sensor,INPUT);
  pinMode(debugLED, OUTPUT);
  pinMode(directionNum, OUTPUT);
  pinMode(stepNum, OUTPUT);

  pinMode(directionMin, OUTPUT);
  pinMode(stepMin, OUTPUT);

  pinMode(directionTund, OUTPUT);
  pinMode(stepTund, OUTPUT);

  pinMode(nsleep, OUTPUT);
  digitalWrite(nsleep, HIGH); // Nsleep High
  digitalWrite(LEDH, HIGH); // Pin High

  // LED riba
  FastLED.addLeds<WS2812, LED_DATA_PIN, RGB>(leds, NUM_LEDS);

  // LCD
  lcd.begin(16, 2);
  lcd.clear();
  /*
  lcd.print("LCD");
  lcd.setCursor(0, 1);
  lcd.print("T");
  lcd.print((char)153);
  lcd.print((char)153);
  lcd.print("tab");
*/

  // Hotspot setup

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Hotspot IP address: ");
  Serial.println(IP);
  // Display IP address on LCD
  lcd.setCursor(0, 0);
  lcd.print("IP: ");
  lcd.print(IP);
  // HTML leht
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", "<html><body>\
      <form action='/connect' method='POST'>\
        SSID: <input type='text' name='ssid'><br>\
        Password: <input type='password' name='password'><br>\
        <input type='submit' value='Connect'>\
      </form>\
    </body></html>");
  });

  // WIFI data from user
  server.on("/connect", HTTP_POST, [](AsyncWebServerRequest *request){
    String ssid, password;
    if(request->hasParam("ssid", true) && request->hasParam("password", true)){
      ssid = request->getParam("ssid", true)->value();
      password = request->getParam("password", true)->value();

      // Connect to the specified WiFi network
      WiFi.begin(ssid.c_str(), password.c_str());
      while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Ühendan WiFi-ga...");
      }

      // Initialize and synchronize time
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
      while (time(nullptr) <= 0) {
        delay(1000);
        Serial.println("Waiting for time synchronization...");
      }
      Serial.println("WiFi ühendus loodud!");
      lcd.setCursor(0, 0);
      lcd.print("Ühendus olemas!");
      request->redirect("/");
    }
    else {
      request->send(400, "text/plain", "Bad Request");
    }
  });

  // Start server 
  server.begin();
}

void loop()
{
  // Motor control
  
  digitalWrite(directionNum, HIGH);
  moveStepper(numberOfSteps, 50, 2, stepNum);
  digitalWrite(directionNum, LOW);
  moveStepper(numberOfSteps, 50, 2, stepNum);
  
  // LED riba
  leds[0] = CHSV(241,255,100);
  leds[1] = CRGB(0,255,0);
  leds[2] = CRGB(0,0,255);
  leds[3] = CRGB(255,0,255);
  FastLED.show();
  
  if (Serial.available() > 0) {
          String input = Serial.readStringUntil('\n');
          if (input == "on") {
          digitalWrite(LEDH, HIGH);
          Serial.println("On");
          } else if (input == "off") {
          digitalWrite(LEDH, LOW);
          Serial.println("Off");

          } 
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
  delay(500);
  if (digitalRead(nupp) == HIGH) {
    digitalWrite(debugLED, HIGH);
  } else {
    digitalWrite(debugLED, LOW);
  }
}

void moveStepper(int steps, int pulseWidthMicros, int millisBetweenSteps, int stepPin)
{
  for (int n = 0; n < steps; n++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(pulseWidthMicros);
    digitalWrite(stepPin, LOW);

    delay(millisBetweenSteps);
  }
}