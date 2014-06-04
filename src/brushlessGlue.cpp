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

int pwmInit() {
  timer0_init();
  return 0;
}

int pwmStart() {
  timer0_start() ;
  return 0;
}

int pwmStop() {
  timer0_stop();
  return 0;
}

int pwmSetFrequency(unsigned int val) {
  timer0_setFrequency(val);
  return 0;
}

int pwmSetDuty(int val) {
  timer0_setDuty(val);
  return 0;
}

int pwmSetPrescaler(int prescaler) {
  timer0_setPrescaler(prescaler);
  return 0;
}

unsigned int pwmGetFrequency() {
  return timer0_getFrequency();
}

int pwmGetDuty() {
  return timer0_getDuty();
}

int pwmGetPrescaler() {
  return timer0_getPrescaler();
}


