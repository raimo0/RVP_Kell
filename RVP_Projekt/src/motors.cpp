#include <Arduino.h>

const int directionNum = 33;
const int stepNum= 32;

const int directionTund = 26;
const int stepTund = 25;

const int directionMin = 14;
const int stepMin = 27;

const int numberOfSteps = 1000;
const int kaugus = 4;
const int nsleep = 23;

const int sammuArv = 0;

void moveStepper(int steps, int pulseWidthMicros, int millisBetweenSteps, int stepPin){
  digitalWrite(nsleep, HIGH); // Nsleep High
  for (int n = 0; n < steps; n++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(pulseWidthMicros);
    digitalWrite(stepPin, LOW);

    delay(millisBetweenSteps);
  }
  digitalWrite(nsleep, LOW); // Nsleep LOW
}

void plaadiLiigutamine(){
  digitalWrite(directionNum, HIGH);
  moveStepper(numberOfSteps, 50, 2, stepNum);
  digitalWrite(directionNum, LOW);
  moveStepper(numberOfSteps, 50, 2, stepNum);
}

void liigutaMinutiMootor(String suund, int sammuArv = 200){
  if (suund == "vasak"){
    digitalWrite(directionMin, LOW);
  } else {
    digitalWrite(directionMin, HIGH);
  }
  moveStepper(sammuArv, 50, 2, stepMin);
}

void liigutaTunniMootor(String suund, int sammuArv = 200){
  if (suund == "vasak"){
    digitalWrite(directionMin, LOW);
  } else {
    digitalWrite(directionMin, HIGH);
  }
  moveStepper(sammuArv, 50, 2, stepMin);
}

void motorSetup() {
    pinMode(directionNum, OUTPUT);
    pinMode(stepNum, OUTPUT);

    pinMode(directionMin, OUTPUT);
    pinMode(stepMin, OUTPUT);

    pinMode(directionTund, OUTPUT);
    pinMode(stepTund, OUTPUT);

    pinMode(nsleep, OUTPUT);
}

