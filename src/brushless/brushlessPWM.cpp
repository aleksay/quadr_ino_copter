#include <brushlessPWM.h>

void pwmInit() { timer0_init(); }

void pwmStart() { timer0_start(); }

void pwmStop() { timer0_stop(); }

void pwmSetFrequency(uint32_t Hz) { timer0_setFrequency(Hz); }

void pwmSetDuty(uint8_t duty) { timer0_setDuty(duty); }

void pwmSetPrescaler(uint16_t prescaler) { timer0_setPrescaler(prescaler); }

uint16_t pwmGetFrequency() { return timer0_getFrequency(); }

uint8_t pwmGetDuty() { return timer0_getDuty(); }

uint16_t pwmGetPrescaler() { return timer0_getPrescaler(); }
