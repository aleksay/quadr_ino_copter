#ifndef BRUSHLESS_GLUE_H
#define BRUSHLESS_GLUE_H

#include "avrTimer0.h"
#include "avrTimer1.h"

int automaInit();
int automaStart(uint16_t Hz);
int automaStop();
int automaSetFrequency(int freqHz);
int automaSetDuty(int val);
int automaSetPrescaler(int prescaler);
int automaGetFrequency();
int automaGetDuty();
int automaGetTop();
int automaGetPrescaler();
int8_t pwmInit();
int8_t pwmStart();
int8_t pwmStop();
int8_t pwmSetFrequency(uint16_t Hz);
int8_t pwmSetDuty(uint8_t duty);
int8_t pwmSetPrescaler(uint16_t prescaler);
uint16_t pwmGetFrequency();
uint8_t pwmGetDuty();
uint16_t pwmGetPrescaler();


#endif //HEADER
