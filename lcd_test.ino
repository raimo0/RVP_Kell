#include <LiquidCrystal.h>
LiquidCrystal lcd(21,2,19,18,5,17,16);

void setup() {
  pinMode(22, OUTPUT); //Debug led
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("LCD");

  lcd.setCursor(0,1); 
  lcd.print ("T");
  lcd.print ((char)153);
  lcd.print ((char)153);
  lcd.print ("tab");
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(22, HIGH);  
  delay(1000);                   
  digitalWrite(22, LOW); 
  delay(1000);       
}
