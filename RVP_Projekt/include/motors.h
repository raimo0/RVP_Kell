#ifndef MOTORS_H
#define MOTORS_H

//

void kuvaMinut(int minut);
void kuvaTund(int tund, int minut, int eelminetund);
//

const int minutiSamm = 200;
extern const int numberOfSteps;
extern const int minutAlgusesse;
extern const int tundAlgusesse;
extern const int sammuArv;
// int tunniSammud[25] = {0, 250, 250, 250, 250, 400, 300, 350, 300, 300, 250, 250, 200, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// const int minutiSammud[8] = {-400,-500,-450,-450,-500,-450,-500};
// int minutiSammud[90] = {-40, -40, -40, -40, -40, -40, -40, -40, -40, -40, -40, -40, -40, -40, -40, -40, -40, -40, -40, -40, -50, -50, -50, -50, -50, -50, -50, -50, -50, -50, -45, -45, -45, -45, -45, -45, -45, -45, -45, -45, -45, -45, -45, -45, -45, -45, -45, -45, -45, -45, -50, -50, -50, -50, -50, -50, -50, -50, -50, -50, -45, -45, -45, -45, -45, -45, -45, -45, -45, -45, -45, -45, -45, -45, -45, -45, -45, -45, -45, -45, -50, -50, -50, -50, -50, -50, -50, -50, -50, -50};
const int test[90] = {};
void moveStepper(int steps, int pulseWidthMicros, int millisBetweenSteps, int stepPin);
void motorSetup(void);
void plaadiLiigutamine(String suund);
void liigutaMinutiMootor(int sammuArv);
void liigutaTunniMootor(int sammuArv);
void tundStarti(void);
void minutStarti(void);

#endif // MOTORS_H