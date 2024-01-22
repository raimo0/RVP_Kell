#ifndef MOTORS_H
#define MOTORS_H

extern const int minutiSamm;
extern const int directionNum;
extern const int stepNum;
extern const int directionMin;
extern const int stepMin;
extern const int directionTund;
extern const int stepTund;
extern const int nsleep;
extern const int numberOfSteps;
extern const int minutAlgusesse;
extern const int sammuArv;


void moveStepper(int steps, int pulseWidthMicros, int millisBetweenSteps, int stepPin);
void motorSetup(void);
void plaadiLiigutamine();
void liigutaMinutiMootor(String suund, int sammuArv);
void liigutaTunniMootor(String suund, int sammuArv);



#endif // MOTORS_H
