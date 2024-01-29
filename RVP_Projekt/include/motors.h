#ifndef MOTORS_H
#define MOTORS_H

// extern const int numberOfSteps;
// extern const int minutAlgusesse;
// extern const int tundAlgusesse;
// extern const int sammuArv;

#define DIR_NUM 33
#define STEP_NUM 32
#define DIR_TUND 26
#define STEP_TUND 25
#define DIR_MIN 14
#define STEP_MIN 27
#define NSLEEP 23
#define KONTROLL_NUPP 35

void kuvaMinut(int minut);
void kuvaTund(int tund, int minut, int eelmine_tund);
void moveStepper(int steps, int pulseWidthMicros, int millisBetweenSteps, int stepPin);
void motorSetup(void);
void plaadiLiigutamine(String suund, int steps);
void liigutaMinutiMootor(int sammuArv);
void liigutaTunniMootor(int sammuArv);
void tundStarti(void);
void minutStarti(void);
void mootoridAlgusesse(void);

#endif // MOTORS_H