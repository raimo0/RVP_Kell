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
extern const int minutAlgusesse;
extern const int tundAlgusesse;
extern const int sammuArv;
int tunniSammud[25] = {0,250,250,250,250,400,300,350,300,300,250,250,200,0,0,0,0,0,0,0,0,0,0,0,0};
int tunniSammudTopelt[25] = {0,0,0,0,0,0,0,0,0,0,0,0,400,250,250,250,250,300,300,300,300,300,200,200,200};
//const int minutiSammud[8] = {-400,-400,-500,-450,-450,-500,-450,-500};
const int minutiSammud[120] = {-40,-40,-40,-40,-40,-40,-40,-40,-40,-40,-40,-40,-40,-40,-40,-40,-40,-40,-40,-40,-50,-50,-50,-50,-50,-50,-50,-50,-50,-50,-45,-45,-45,-45,-45,-45,-45,-45,-45,-45,-45,-45,-45,-45,-45,-45,-45,-45,-45,-45,-50,-50,-50,-50,-50,-50,-50,-50,-50,-50,-45,-45,-45,-45,-45,-45,-45,-45,-45,-45,-45,-45,-45,-45,-45,-45,-45,-45,-45,-45,-50,-50,-50,-50,-50,-50,-50,-50,-50,-50};

void moveStepper(int steps, int pulseWidthMicros, int millisBetweenSteps, int stepPin);
void motorSetup(void);
void plaadiLiigutamine(String suund);
void liigutaMinutiMootor(int sammuArv);
void liigutaTunniMootor(int sammuArv);
void tundStarti(void);
void minutStarti(void);



#endif // MOTORS_H