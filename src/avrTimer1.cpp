
#include "avrTimer1.h"


// VARIABLES
unsigned int timer1_frequency;
unsigned int timer1_top;
int timer1_duty;
int timer1_dutyVal;
int timer1_prescaler;

// FUNCTIONS
int timer1_init() {

  //configure timer1
  timer1_fastPwm_ocr1atop_init();
  
  //initialize timer1 global variable
  timer1_duty = DEFAULT_T1_INIT_DUTY;
  timer1_prescaler = DEFAULT_T1_INIT_PRESCALER;   
  timer1_setFrequency(DEFAULT_T1_INIT_FREQUENCY);
  
  return 0;
}

int timer1_fastPwm_icr1top_init() {


  SET_TIMER1_PINB;
  SET_TIMER1_FREQUENCY_ICR1TOP(DEFAULT_T1_INIT_FREQUENCY);

  SET_TIMER1_DUTY_CHAN_B(DEFAULT_T1_INIT_DUTY);

  TIMER1_RESET;
  SET_TIMER1_PINOUT(B);  
  SET_TIMER1_MODE_FASTPWM_ICR1;
  SET_TIMER1_PINB_NOTINVERTING(0);
  SET_TIMER1_INTERRUPT_OUTPUTCOMPARE_B;
  //SET_TIMER1_PRESCALER_1;

  return 0;
}

int timer1_fastPwm_ocr1atop_init() {
  SET_TIMER1_INTERRUPT_OUTPUTCOMPARE_A;
  //SET_TIMER1_INTERRUPT_OUTPUTCOMPARE_B;
  //SET_TIMER1_INTERRUPT_OVERFLOW;
  TIMER1_RESET;

  SET_TIMER1_PINB;

  //SET_TIMER1_PINOUT(B);
  SET_TIMER1_MODE_FASTPWM_OCR1A;
  SET_TIMER1_PINB_NOTINVERTING(0);



  return 0;
}

int timer1_start() {
  //debug("prescaler set to: %d",prescaler);

  timer1_setPrescaler(timer1_prescaler);
  return 0;
}
int timer1_start(int _prescaler) {
  //debug("prescaler set to: %d",prescaler);
  timer1_setPrescaler(_prescaler);
  return 0;
}

//stop timer1 by removing the prescaler
int timer1_stop(){ 
  SET_TIMER1_STOP;
  AUTOMA_OPEN_INVERTER;
  return 0;
}

int timer1_setPrescaler(int _prescaler){



  switch(_prescaler) {

  case 1:
    SET_TIMER1_PRESCALER_1;
    timer1_prescaler = 1;
    debug("prescaler set to: %d",timer1_prescaler);
    return 0;
  case 8:
    SET_TIMER1_PRESCALER_8;
    timer1_prescaler = 8;
    debug("prescaler set to: %d",timer1_prescaler);
    return 0;
  case 64:
    SET_TIMER1_PRESCALER_64;
    timer1_prescaler = 64;
    debug("prescaler set to: %d",timer1_prescaler);
    return 0;	
  case 256:
    SET_TIMER1_PRESCALER_256;
    timer1_prescaler = 256;
    debug("prescaler set to: %d",timer1_prescaler);
    return 0;
  case 1024:
    SET_TIMER1_PRESCALER_1024;
    timer1_prescaler = 1024;
    debug("prescaler set to: %d",timer1_prescaler);
    return 0;			
  } 

  return 1;
}

//convert to TOP value
int timer1_Hz2top(int freqHz) {  
  //debug("%u,%d,%d",F_CPU,timer1_getPrescaler(),freqHz);
  return floor(F_CPU/(timer1_getPrescaler() * freqHz)-1);
}


int timer1_setFrequency(unsigned int freqHz) {

  // check value
  if (freqHz == timer1_frequency) {
    //log_warn("Value unchanged!");
    return -1;
  }
  //update global variable frequency
  timer1_frequency = freqHz;

  //convert value to microcontroller TOP
  unsigned int top = timer1_Hz2top(freqHz);
  //debug("%d",timer1_top);

  //check TOP consistency
  if (top < 245 || top > 65535){ //limiti x non bloccare il microcontrollore non avendo il controllo sul prescaler
    log_err("Bad TOP!");
    return -1;
  }
  if (timer1_top == top) {
    log_warn("Value unchanged!");
    return -1;
  }

  int zDuty = -10;

  //set new value on the register
  SET_TIMER1_FREQUENCY_OCR1ATOP(top);

  //update global variable top
  timer1_top = top;

  zDuty = timer1_setDuty(timer1_duty);

  return zDuty;
}

int timer1_setDuty(int val) {

  if (val < 0 || val > 100){
    log_err("bad duty");
    return -1;
  }
  timer1_duty = val;
  timer1_dutyVal = avrMap(timer1_duty, 0, 100, 0, timer1_top);
  SET_TIMER1_DUTY_CHAN_B(timer1_dutyVal);
  return 0;

}

unsigned int timer1_getFrequency() {
  return timer1_frequency;
}

unsigned int timer1_getTop() {
  return timer1_top;
}
int timer1_getDuty() {
  return timer1_duty;
}

int timer1_getPrescaler(){
  return timer1_prescaler;
}

void timer1_timer1_ovf_handler(){
  SET_TIMER1_FREQUENCY_OCR1ATOP(timer1_frequency);
  SET_TIMER1_DUTY_CHAN_B(timer1_dutyVal);
}




