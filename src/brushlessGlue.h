#ifndef BRUSHLESS_GLUE_H
#define BRUSHLESS_GLUE_H

#include "avrTimer0.h"
#include "avrTimer1.h"

int automaInit();
int automaStart();
int automaStop();
int automaSetFrequency(int freqHz);
int automaSetDuty(int val);
int automaSetPrescaler(int prescaler);
int automaGetFrequency();
int automaGetDuty();
int automaGetTop();
int automaGetPrescaler();
int pwmInit();
int pwmStart();
int pwmStop();
int pwmSetFrequency(unsigned int val);
int pwmSetDuty(int val);
int pwmSetPrescaler(int prescaler);
unsigned int pwmGetFrequency();
int pwmGetDuty();
int pwmGetPrescaler();

#endif //HEADER
