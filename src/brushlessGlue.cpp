#include "brushlessGlue.h"

int automaInit(){
  timer1_init();
  return 0;
}

int automaStart(){
  timer1_start();
  return 0;
}

int automaStop(){

  //TODO
  timer1_stop();
  return 0;
}

int automaSetFrequency(int freqHz){
  timer1_setFrequency(freqHz);
  return 0;
}

int automaSetDuty(int val){
  timer1_setDuty(val);
  return 0;
}

int automaSetPrescaler(int prescaler){
  timer1_setPrescaler(prescaler);
  return 0;
}

int automaGetFrequency(){
  timer1_getFrequency();
  return 0;
}

int automaGetDuty(){
  timer1_getDuty();
  return 0;
}

int automaGetTop(){
  timer1_getTop();
  return 0;
}

int automaGetPrescaler(){
  timer1_getPrescaler();
  return 0;
}

int pwmInit(){
  timer0_init();
  return 0;
}

int pwmStart(){
  timer0_start() ;
  return 0;
}

int pwmStop(){
  // TODO
  return 0;
}

int pwmSetFrequency(unsigned int val){
  timer0_setFrequency(val);
  return 0;
}

int pwmSetDuty(int val){
  timer0_setDuty(val);
  return 0;
}

int pwmSetPrescaler(int prescaler){
  timer0_setPrescaler(prescaler);
  return 0;
}

unsigned int pwmGetFrequency(){
  return timer0_getFrequency();
}

int pwmGetDuty(){
  return timer0_getDuty();
}

int pwmGetPrescaler(){
  return timer0_getPrescaler();
}


