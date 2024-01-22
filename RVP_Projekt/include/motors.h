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
const int tunniSammud[25] = {0,250,250,250,250,400,300,350,300,300,250,250,200,0,0,0,0,0,0,0,0,0,0,0,0};
const int tunniSammudTopelt[25] = {0,0,0,0,0,0,0,0,0,0,0,0,400,250,250,250,250,300,300,300,300,300,200,200,200};
const int minutiSammud[8] = {-50,-350,-500,-400,-500,-450,-500,-500};

void moveStepper(int steps, int pulseWidthMicros, int millisBetweenSteps, int stepPin);
void motorSetup(void);
void plaadiLiigutamine(String suund);
void liigutaMinutiMootor(int sammuArv);
void liigutaTunniMootor(int sammuArv);



#endif // MOTORS_H