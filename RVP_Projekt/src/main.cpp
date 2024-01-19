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
byte directionPin = 33;
byte stepPin = 32;
int numberOfSteps = 400;
int nsleep = 23;

// Variables

const char *ssid = "Hotspot";
const char *password = "123456789";
byte LEDH = 13;
const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600;

AsyncWebServer server(80);
// put function declarations here:

void moveStepper(int, int, int);

void setup()
{
  Serial.begin(115200);
  pinMode(LEDH,OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(directionPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(nsleep, OUTPUT);
  digitalWrite(nsleep, HIGH); // Nsleep High

  // LED riba
  FastLED.addLeds<WS2813, LED_DATA_PIN, RGB>(leds, NUM_LEDS);

  // LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("LCD");

  lcd.setCursor(0, 1);
  lcd.print("T");
  lcd.print((char)153);
  lcd.print((char)153);
  lcd.print("tab");

  // Hotspot setup

  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Hotspot IP address: ");
  Serial.println(IP);

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
        Serial.println("Connecting to WiFi...");
      }

      // Initialize and synchronize time
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
      while (time(nullptr) <= 0) {
        delay(1000);
        Serial.println("Waiting for time synchronization...");
      }

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
  /*
  digitalWrite(directionPin, HIGH);
  moveStepper(numberOfSteps, 50, 2);
  digitalWrite(directionPin, LOW);
  moveStepper(numberOfSteps, 50, 2);

  // LED riba
  leds[0] = CRGB::Blue;
  leds[1] = CRGB::Green;
  leds[2] = CRGB::Red;
  leds[3] = CRGB::Yellow;
  FastLED.show();
  */
 Serial.println("Tere");
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
  delay(500);
}

void moveStepper(int steps, int pulseWidthMicros, int millisBetweenSteps)
{
  for (int n = 0; n < steps; n++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(pulseWidthMicros);
    digitalWrite(stepPin, LOW);

    delay(millisBetweenSteps);
  }
}