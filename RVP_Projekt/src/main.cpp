#include <Arduino.h>
#include <LiquidCrystal.h>
#include <FastLED.h>

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

// put function declarations here:

void moveStepper(int, int, int);

void setup()
{
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
}

void loop()
{
  // Motor control
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