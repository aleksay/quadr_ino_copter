#include "brushlessGlue.h"

int automaInit() {
  timer1_init();
  return 0;
}

int automaStart(uint16_t Hz) {
  timer1_start(Hz);
  return 0;
}

int automaStop() {
  timer1_stop();
  return 0;
}

int automaSetFrequency(int freqHz) {
  timer1_setFrequency(freqHz);
  return 0;
}

int automaSetDuty(int val) {
  timer1_setDuty(val);
  return 0;
}

int automaGetFrequency() {
  return timer1_getFrequency();
}

int automaGetDuty() {
  return timer1_getDuty();;
}

int automaGetTop() {
  return timer1_getTop();
}

int8_t pwmInit() {
  return timer0_init();
}

int8_t pwmStart() {
  return timer0_start() ;
}

int8_t pwmStop() {
  return timer0_stop();
}

int8_t pwmSetFrequency(uint16_t Hz) {
  return timer0_setFrequency(Hz);
}

int8_t pwmSetDuty(uint8_t duty) {
  return timer0_setDuty(duty);
}

int8_t pwmSetPrescaler(uint16_t prescaler) {
  return timer0_setPrescaler(prescaler);
}

uint16_t pwmGetFrequency() {
  return timer0_getFrequency();
}

uint8_t pwmGetDuty() {
  return timer0_getDuty();
}

uint16_t pwmGetPrescaler() {
  return timer0_getPrescaler();
}


