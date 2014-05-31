#include "avrTimer0.h"

// GLOBALS

unsigned int timer0_frequency;
int timer0_duty;
unsigned int timer0_dutyVal;
int timer0_prescaler;


int timer0_init(){
  //configure timer0
  timer0_phasecorrect_ocr0atop_init();

  //initialize timer1 global variable
  timer0_duty = DEFAULT_T0_INIT_DUTY;
  timer0_prescaler = DEFAULT_T0_INIT_PRESCALER;   
  timer0_setFrequency(DEFAULT_T0_INIT_FREQUENCY);
  return 0 ;
}

void timer0_phasecorrect_ocr0atop_init(){
  //cli();
  SET_TIMER0_PINB;
  TIMER0_RESET;

  SET_TIMER0_PINOUT(B);  
  SET_TIMER0_MODE_PHASE_CORRECT_OCR0A;
  SET_TIMER0_PINB_NOTINVERTING(0);
  //SET_TIMER0_INTERRUPT_OUTPUTCOMPARE_B;
  //SET_TIMER0_INTERRUPT_OVERFLOW;
  //sei();
}

int timer0_start() {
  timer0_setPrescaler(timer0_prescaler);

  return 0;
}
int timer0_start(int _prescaler) {
  timer0_setPrescaler(_prescaler);
  return 0;
}

//stop timer1 by removing the prescaler
int timer0_stop(){ 
  
  timer0_setDuty(0);
  // BISOGNA ASPETTARE DEL TEMPO UTILIZZANDO GET TIME IN TIME.CPP prima di settare questo

  //SET_TIMER0_STOP;
  return 0;
}

int timer0_setPrescaler(int _prescaler){

  switch(_prescaler) {

  case 1:
    SET_TIMER0_PRESCALER_1;
    timer0_prescaler = 1;
    debug("prescaler set to: %d",timer0_prescaler);
    return 0;
  case 8:
    SET_TIMER0_PRESCALER_8;
    timer0_prescaler = 8;
    debug("prescaler set to: %d",timer0_prescaler);
    return 0;
  case 64:
    SET_TIMER0_PRESCALER_64;
    timer0_prescaler = 64;
    debug("prescaler set to: %d",timer0_prescaler);
    return 0;	
  case 256:
    SET_TIMER0_PRESCALER_256;
    timer0_prescaler = 256;
    debug("prescaler set to: %d",timer0_prescaler);
    return 0;
  case 1024:
    SET_TIMER0_PRESCALER_1024;
    timer0_prescaler = 1024;
    debug("prescaler set to: %d",timer0_prescaler);
    return 0;			
  }
  return 1;
}

int timer0_getPrescaler(){
  return timer0_prescaler;
}


int timer0_setFrequency(unsigned int val) {
  if (val < 0 || val > 254){
    log_err("invalid value:%u",val);
    return -1;
  }
  if (val == timer0_frequency){
    log_warn("value unchanged");
    return -1;
  }

  int zDuty = -10;


  timer0_frequency = val;       

  SET_TIMER0_FREQUENCY(timer0_frequency);
  zDuty = timer0_setDuty(timer0_duty);


  return zDuty;
}

int timer0_setDuty(int val) {

  if (val < 0 || val >= 100)
  {
    log_err("invalid value:%d",val);
    return -1;
  }

  timer0_duty = val;
  timer0_dutyVal = avrMap(timer0_duty, 0, 100, 0, timer0_frequency);
  SET_TIMER0_DUTY(timer0_dutyVal);
  return 0;
}

unsigned int timer0_getFrequency() {
  return timer0_frequency;
}
int timer0_getDuty() {
  return timer0_duty;
}








