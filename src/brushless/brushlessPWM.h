#ifndef BRUSHLESS_PWM_H
#define BRUSHLESS_PWM_H

#include <avrTimer0.h>

#define DEFAULT_T0_INIT_FREQUENCY 20000 // ancora in TOP OCRA
#define DEFAULT_T0_INIT_DUTY 1
#define DEFAULT_T0_INIT_PRESCALER 1

#define RAMP_FIN_DUTY_T0 90

void pwmInit (void);
void pwmStart (void);
void pwmStop (void);
void pwmSetFrequency (uint32_t Hz);
void pwmSetDuty (uint8_t duty);
void pwmSetPrescaler (uint16_t prescaler);
uint16_t pwmGetFrequency (void);
uint8_t pwmGetDuty (void);
uint16_t pwmGetPrescaler (void);

#endif // HEADER
