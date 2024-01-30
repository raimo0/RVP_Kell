#include <Arduino.h>
#include "motors.h"

int stepperiKiirus = 1000;
int minutiSteppideArv = 0;
int tunniSteppideArv = 0;
bool esimeneKord = true;

const int tunniSammud[26] = {-120, 150, 270, 250, 300, 330, 340, 350, 320, 310, 270, 270, 200, /**/ 0, 200, 240, 300, 340, 330, 320, 350, 300, 250, 260, 150, 210};
const int minutiSammud[8] = {0, 470, 450, 450, 450, 450, 500};

void moveStepper(int samme, int pulsiPikkusMikrodes, int pausMikrodes, int pin)
{
  digitalWrite(NSLEEP, HIGH); // Nsleep High
  for (int n = 0; n < samme; n++)
  {
    digitalWrite(pin, HIGH);

    delayMicroseconds(pulsiPikkusMikrodes);
    digitalWrite(pin, LOW);
    delayMicroseconds(pausMikrodes);
  }
  digitalWrite(NSLEEP, LOW); // Nsleep LOW
}

void plaadiLiigutamine(String suund, int samme)
{
  if (suund == "vasak")
  {
    digitalWrite(DIR_NUM, HIGH);
  }
  else
  {
    digitalWrite(DIR_NUM, LOW);
  }
  moveStepper(samme, 50, stepperiKiirus, STEP_NUM);
}

void liigutaMinutiMootor(int sammuArv = 200)
{
  // Serial.printf("Liigub minuti stepper: %i\n", sammuArv);
  if (sammuArv < 0)
    digitalWrite(DIR_MIN, LOW);
  else
    digitalWrite(DIR_MIN, HIGH);
  moveStepper(abs(sammuArv), 50, stepperiKiirus, STEP_MIN);
  minutiSteppideArv += sammuArv;
}

void liigutaTunniMootor(int sammuArv = 200)
{
  // Serial.printf("Liigub tunni stepper: %i\n", sammuArv);
  if (sammuArv < 0)
    digitalWrite(DIR_TUND, LOW);
  else
    digitalWrite(DIR_TUND, HIGH);
  moveStepper(abs(sammuArv), 50, stepperiKiirus, STEP_TUND);
  tunniSteppideArv += sammuArv;
}

void kuvaMinut(int minut)
{
  int minutikumnendik = (int)(minut / 10);
  int steppePraeguseMinutiniKumneni = 0;
  for (int i = 0; i < minutikumnendik + 1; i++)
  {
    steppePraeguseMinutiniKumneni += minutiSammud[i];
  }
  // Serial.printf("Minuti: %i, %i, %i\n", (steppePraeguseMinutiniKumneni + (minutiSammud[minutikumnendik] / 10) * (minut % 10)), minutiSteppideArv, (minutiSammud[minutikumnendik] / 10) * (minut % 10));
  liigutaMinutiMootor((steppePraeguseMinutiniKumneni + (minutiSammud[minutikumnendik + 1] / 10) * (minut % 10)) - minutiSteppideArv);
}

void kuvaTund(int tund, int minut, int eelminetund)
{
  if (esimeneKord)
  {
    if (tund < 12)
    {
      plaadiLiigutamine("parem", 1100);
    }
    esimeneKord = false;
  }
  // Seier algusesse kui kell läheb üle 12 või üle 24
  if ((eelminetund < 12 && tund >= 12) || (eelminetund >= 12 && tund < 12))
  {
    liigutaTunniMootor(-tunniSteppideArv);
    tunniSteppideArv = 0;

    // Numbri plaadi liigutamine
    if (tund >= 12)
    {
      plaadiLiigutamine("vasak", 1100);
    }
    else if (tund < 12)
    {
      plaadiLiigutamine("parem", 1100);
    }
  }

  int steppePraeguseTunnini = 0;
  int i;
  if (tund < 12)
    i = 0;
  else
    i = 12;
  if (tund != 12)
  {
    for (; i <= tund; i++)
    {
      steppePraeguseTunnini += tunniSammud[i];
    }
  }
  Serial.printf("Tunni: %i, %i, %i\n", (steppePraeguseTunnini + tunniSammud[tund + 1] / 60 * minut), tunniSteppideArv, (steppePraeguseTunnini + tunniSammud[tund + 1] / 60 * minut) - tunniSteppideArv);
  liigutaTunniMootor((steppePraeguseTunnini + tunniSammud[tund + 1] / 60 * minut) - tunniSteppideArv);
}

void mootoridAlgusesse()
{
  while (digitalRead(KONTROLL_NUPP) == 1)
  {
    liigutaTunniMootor(50);
    liigutaMinutiMootor(-50);
    plaadiLiigutamine("parem", 50);
    Serial.println(digitalRead(KONTROLL_NUPP));
  }
  // Tunnid algusesse
  while (digitalRead(KONTROLL_NUPP) == 0)
  {
    liigutaTunniMootor(-20);
  }
  liigutaTunniMootor(100);
  while (digitalRead(KONTROLL_NUPP) == 0)
  {
    liigutaMinutiMootor(20);
  }
  liigutaMinutiMootor(-100);

  while (digitalRead(KONTROLL_NUPP) == 0)
  {
    plaadiLiigutamine("vasak", 50);
  }
  plaadiLiigutamine("parem", 50);
  liigutaMinutiMootor(-3050);
  liigutaTunniMootor(150);
  minutiSteppideArv = 0;
  tunniSteppideArv = 0;
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
  pinMode(KONTROLL_NUPP, INPUT);
}
