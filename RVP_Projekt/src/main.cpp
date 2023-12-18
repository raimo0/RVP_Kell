#include <Arduino.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(21, 2, 19, 18, 5, 17, 16);
// put function declarations here:
int myFunction(int, int);

void setup()
{
  // put your setup code here, to run once:
  pinMode(22, OUTPUT);
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
}