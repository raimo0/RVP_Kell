#ifndef MOTORS_H
#define MOTORS_H



extern int eelmine_tund;
extern const int numberOfSteps;
extern const int minutAlgusesse;
extern const int tundAlgusesse;
extern const int sammuArv;




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