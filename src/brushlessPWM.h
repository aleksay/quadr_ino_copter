#ifndef BRUSHLESS_PWM_H
#define BRUSHLESS_PWM_H

#include "avrTimer0.h"

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
