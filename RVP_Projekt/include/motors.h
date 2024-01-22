#ifndef MOTORS_H
#define MOTORS_H

const int minutiSamm = 200;
extern const int directionNum;
extern const int stepNum;
extern const int directionMin;
extern const int stepMin;
extern const int directionTund;
extern const int stepTund;
extern const int nsleep;
extern const int numberOfSteps;
const int minutAlgusesse = 1200;
extern const int sammuArv;


void moveStepper(int steps, int pulseWidthMicros, int millisBetweenSteps, int stepPin);
void motorSetup(void);
void plaadiLiigutamine(String suund);
void liigutaMinutiMootor(int sammuArv);
void liigutaTunniMootor(int sammuArv);



#endif // MOTORS_H