#include <Arduino.h>
#include "motors.h"

const int numberOfSteps = 1000;
const int kaugus = 4;
const int sammuArv = 0;
const int minutAlgusesse = 3500;
const int tundAlgusesse = -5000;

void moveStepper(int steps, int pulseWidthMicros, int millisBetweenSteps, int stepPin)
{
  digitalWrite(NSLEEP, HIGH); // Nsleep High
  for (int n = 0; n < steps; n++)
  {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(pulseWidthMicros);
    digitalWrite(stepPin, LOW);

    delay(millisBetweenSteps);
  }
  digitalWrite(NSLEEP, LOW); // Nsleep LOW
}

void plaadiLiigutamine(String suund)
{
  if (suund == "vasak")
  {
    digitalWrite(DIR_NUM, HIGH);
  }
  else
  {
    digitalWrite(DIR_NUM, LOW);
  }
  moveStepper(numberOfSteps, 50, 2, STEP_NUM);
}

void liigutaMinutiMootor(int sammuArv = 200)
{
  if (sammuArv < 0)
    digitalWrite(DIR_MIN, LOW);
  else
    digitalWrite(DIR_MIN, HIGH);
  moveStepper(abs(sammuArv), 50, 2, STEP_MIN);
  minutiSteppideArv += sammuArv;
}

void kuvaMinut(int minut) // Kui see ei tööta, siis uncommenti alumine variant
{
  liigutaMinutiMootor(minut * minutisSteppe - minutiSteppideArv);
}

/*void kuvaMinut(int minut)
{
  int steppePraeguseMinutiniKümneni = 0;
  for (int i = 0; i < (int)(minut / 10); i++)
  {
    steppePraeguseMinutiniKümneni += minutiSammud[i];
  }
  liigutaMinutiMootor((steppePraeguseMinutiniKümneni + minutiSammud[(int)minut / 10] / 10 * minut) - minutiSteppideArv);
}*/

void liigutaTunniMootor(int sammuArv = 200)
{
  if (sammuArv < 0)
    digitalWrite(DIR_TUND, LOW);
  else
    digitalWrite(DIR_TUND, HIGH);
  moveStepper(abs(sammuArv), 50, 2, STEP_TUND);
  tunniSteppideArv += sammuArv;
}

void kuvaTund(int tund, int minut)
{
  // Seier algusesse kui kell läheb üle 12 või üle 24
  if ((eelmine_tund < 12 && tund >= 12) || (eelmine_tund >= 12 && tund < 12))
  {
    int sammekokku = 0;
    for (int i = 0; i < sizeof(tunniSammud); i++)
    {
      sammekokku += tunniSammud[i];
    }
    liigutaTunniMootor(-sammekokku);
    tunniSteppideArv = 0;
  }

  int steppePraeguseTunnini = 0;
  int i;
  if (tund < 12)
    i = 0;
  else
    i = 12;
  for (i = 0; i <= tund; i++)
  {
    steppePraeguseTunnini += tunniSammud[i];
  }
  liigutaTunniMootor((steppePraeguseTunnini + tunniSammud[tund] / 60 * minut) - tunniSteppideArv);
  eelmine_tund = tund;
}

void tundStarti(void)
{
  liigutaTunniMootor(tundAlgusesse);
}
void minutStarti(void)
{
  liigutaMinutiMootor(minutAlgusesse);
  liigutaMinutiMootor(-3250 - 400); // Peaks olema 0 min?
}

void motorSetup()
{
  pinMode(DIR_NUM, OUTPUT);
  pinMode(STEP_NUM, OUTPUT);

  pinMode(DIR_MIN, OUTPUT);
  pinMode(STEP_MIN, OUTPUT);

  pinMode(DIR_TUND, OUTPUT);
  pinMode(STEP_TUND, OUTPUT);

  pinMode(NSLEEP, OUTPUT);
}
