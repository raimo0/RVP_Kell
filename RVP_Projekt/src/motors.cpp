#include <Arduino.h>
#define DIR_NUM 33
#define STEP_NUM 32
#define DIR_TUND 26
#define STEP_TUND 25
#define DIR_MIN 14
#define STEP_MIN 27
#define NSLEEP 23
#define KONTROLL_NUPP 35

const int numberOfSteps = 1000;
const int sammuArv = 0;
const int minutAlgusesse = 3500;
const int tundAlgusesse = -5000;
int stepperSpeed = 1;
int minutiSteppideArv = 0;
int tunniSteppideArv = 0;
const int tunniSammud[26] = {-100, 225, 250, 280, 260, 330, 370, 320, 330, 300, 290, 250, 250, 175, 200, 280, 260, 320, 470, 270, 330, 300, 250, 250, 200,250};
const int minutiSammud[8] = {0, 470, 450, 450, 450, 450, 500};


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




void plaadiLiigutamine(String suund, int steps){
  if (suund == "vasak")
  {
    digitalWrite(DIR_NUM, HIGH);
  }
  else
  {
    digitalWrite(DIR_NUM, LOW);
  }
  moveStepper(steps, 50, stepperSpeed, STEP_NUM);
}

void liigutaMinutiMootor(int sammuArv = 200)
{
  // Serial.printf("Liigub minuti stepper: %i\n", sammuArv);
  if (sammuArv < 0)
    digitalWrite(DIR_MIN, LOW);
  else
    digitalWrite(DIR_MIN, HIGH);
  moveStepper(abs(sammuArv), 50, stepperSpeed, STEP_MIN);
  minutiSteppideArv += sammuArv;
}

void liigutaTunniMootor(int sammuArv = 200)
{
  if (sammuArv < 0)
    digitalWrite(DIR_TUND, LOW);
  else
    digitalWrite(DIR_TUND, HIGH);
  moveStepper(abs(sammuArv), 50, stepperSpeed, STEP_TUND);
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
  //Serial.printf("Minuti: %i, %i, %i\n", (steppePraeguseMinutiniKumneni + (minutiSammud[minutikumnendik] / 10) * (minut % 10)), minutiSteppideArv, (minutiSammud[minutikumnendik] / 10) * (minut % 10));
  liigutaMinutiMootor((steppePraeguseMinutiniKumneni + (minutiSammud[minutikumnendik] / 10) * (minut % 10)) - minutiSteppideArv);
}


void kuvaTund(int tund, int minut, int eelminetund)
{
  // Seier algusesse kui kell läheb üle 12 või üle 24
  if ((eelminetund < 12 && tund >= 12) || (eelminetund >= 12 && tund < 12))
  {
    liigutaTunniMootor(-tunniSteppideArv);
    tunniSteppideArv = 0;
  }
  int steppePraeguseTunnini = 0;
  int i;
  if (tund < 12)
    i = 0;
  else
    i = 12;
  for (; i <= tund; i++)
  {
    steppePraeguseTunnini += tunniSammud[i];
  }
  // Serial.printf("Tunni: %i, %i, %i\n", (steppePraeguseTunnini + tunniSammud[tund] / 60 * minut), tunniSteppideArv, (steppePraeguseTunnini + tunniSammud[tund] / 60 * minut) - tunniSteppideArv);
  liigutaTunniMootor((steppePraeguseTunnini + tunniSammud[tund] / 60 * minut) - tunniSteppideArv);
  // eelmine_tund = tund;
}

void tundStarti(void){
  
  while (digitalRead(KONTROLL_NUPP) == 0) {
    liigutaTunniMootor(-20);
  }
  liigutaTunniMootor(20);
  tunniSteppideArv = 0;

}

void minutStarti(void)
{
  
  while(digitalRead(KONTROLL_NUPP) == 0){
    liigutaMinutiMootor(20);
  }

  liigutaMinutiMootor(-20);

  liigutaMinutiMootor(-3250 - 400); // Peaks olema 0 min?
  minutiSteppideArv = 0;
}




void mootoridAlgusesse(){
  while (digitalRead(KONTROLL_NUPP) == 1){
    liigutaTunniMootor(50);
    liigutaMinutiMootor(-50);
    plaadiLiigutamine("parem",50);
    Serial.println(digitalRead(KONTROLL_NUPP));

  }
  // Tunnid algusesse
  while (digitalRead(KONTROLL_NUPP) == 0) {
    liigutaTunniMootor(-20);
  }
  liigutaTunniMootor(100);
  while(digitalRead(KONTROLL_NUPP) == 0){
    liigutaMinutiMootor(20);
  }
  liigutaMinutiMootor(-100);

  while(digitalRead(KONTROLL_NUPP) == 0){
    plaadiLiigutamine("vasak",50);
  }
  plaadiLiigutamine("parem",50);
  liigutaMinutiMootor(-3030); // Peaks olema 0 min?
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
