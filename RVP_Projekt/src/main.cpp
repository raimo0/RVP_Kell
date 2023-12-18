#include <Arduino.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(21, 2, 19, 18, 5, 17, 16);
byte directionPin = 33;
byte stepPin = 32;
int numberOfSteps = 400;
int nsleep = 23;



// put function declarations here:
int myFunction(int, int);
void moveStepper(int, int ,int);

void setup()
{
  // put your setup code here, to run once:
  pinMode(22, OUTPUT);
  pinMode(directionPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(nsleep, OUTPUT);
  digitalWrite(nsleep, HIGH);  // Nsleep High

  // pinMode(2, OUTPUT);
  // digitalWrite(2, LOW);
  //  put your setup code here, to run once: RW2
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
  // put your main code here, to run repeatedly:
  digitalWrite(22, HIGH);
  delay(1000);
  digitalWrite(22, LOW);
  delay(1000);

  // Motor control
  digitalWrite(directionPin, HIGH); 
  moveStepper(numberOfSteps, 50, 2);
  digitalWrite(directionPin, LOW); 
  moveStepper(numberOfSteps, 50, 2);


}

void moveStepper(int steps, int pulseWidthMicros, int millisBetweenSteps) {
  for (int n = 0; n < steps; n++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(pulseWidthMicros);
    digitalWrite(stepPin, LOW);

    delay(millisBetweenSteps);
  }
}
