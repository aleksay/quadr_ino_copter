#ifndef Timer_h
#define Timer_h
#include "timer.h"
#include "communicator.h"
#include "comLogger.h"

class timer1: 
public timer
{
public:
  timer1() {

	//initialize timer1 global variable
//    frequency = DEFAULT_T1_INIT_FREQUENCY;
//    duty = DEFAULT_T1_INIT_DUTY;
    prescaler = DEFAULT_T1_INIT_PRESCALER;
	
	//configure timer1
    _timer1_fastPwm_ocr1atop_init();

  }

  int _timer1_fastPwm_icr1top_init() {

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

  int _timer1_fastPwm_ocr1atop_init() {
    SET_TIMER1_INTERRUPT_OUTPUTCOMPARE_A;
    //SET_TIMER1_INTERRUPT_OUTPUTCOMPARE_B;
    //SET_TIMER1_INTERRUPT_OVERFLOW;
    TIMER1_RESET;

    SET_TIMER1_PINB;
	
    setFrequency(DEFAULT_T1_INIT_FREQUENCY);
    setDuty(DEFAULT_T1_INIT_DUTY);

    //SET_TIMER1_PINOUT(B);
    SET_TIMER1_MODE_FASTPWM_OCR1A;
    SET_TIMER1_PINB_NOTINVERTING(0);



    return 0;
  }

  int start() {
    //debug("prescaler set to: %d",prescaler);
    
    setPrescaler(prescaler);
    return 0;
  }
  int start(int _prescaler) {
    //debug("prescaler set to: %d",prescaler);
    setPrescaler(_prescaler);
    return 0;
  }

  //int timer::stop(){ //not working with prescaler
  //	SET_TIMER1_STOP;
  //}

  int setPrescaler(int _prescaler){


    
    switch(_prescaler) {
    
    case 1:
      debug("prescaler set to: %d",prescaler);
      SET_TIMER1_PRESCALER_1;
      prescaler = 1;
      return 0;
    case 8:
      SET_TIMER1_PRESCALER_8;
      prescaler = 8;
      return 0;
    case 64:
      SET_TIMER1_PRESCALER_64;
      prescaler = 64;
      return 0;	
    case 256:
      SET_TIMER1_PRESCALER_256;
      prescaler = 256;
      return 0;
    case 1024:
      SET_TIMER1_PRESCALER_1024;
      prescaler = 1024;
      return 0;			
    } 

    return 1;
  }

	//convert to TOP value
	int Hz2top(int freqHz) {         
	 return floor(F_CPU/(getPrescaler() * freqHz)-1);
	}
  
 
	int setFrequency(unsigned int freqHz) {
	
	// check value
	if (freqHz == frequency) {
	  return -1;
	}
	//update global variable frequency
	frequency = freqHz;
		
	//convert value to microcontroller TOP
	unsigned int _top = Hz2top(freqHz);
        //debug("%d",_top);
	
	//check TOP consistency
	if (_top < 245 || _top > 65535){ //limiti x non bloccare il microcontrollore non avendo il controllo sul prescaler
	  return -1;
	}
	if (_top == top) {
	  return -1;
	}

	int zDuty = -10;
         
	//set new value on the register
        SET_TIMER1_FREQUENCY_OCR1ATOP(_top);
	
	//update global variable top
	top = _top;
	
    zDuty = setDuty(duty);

    return zDuty;
  }

  int setDuty(int val) {

    if (val < 0 || val > 100)
      return -1;

    duty = val;
    _dutyVal = map(duty, 0, 100, 0, frequency);
    SET_TIMER1_DUTY_CHAN_B(_dutyVal);
    return 0;

  }

  unsigned int getFrequency() {
    return frequency;
  }
  
  unsigned int getTop() {
    return top;
  }
  int getDuty() {
    return duty;
  }
  
  int getPrescaler(){
    return prescaler;
  }
  
  void _timer1_ovf_handler(){
    SET_TIMER1_FREQUENCY_OCR1ATOP(frequency);
    SET_TIMER1_DUTY_CHAN_B(_dutyVal);
  }
private: 
  unsigned int frequency;
  unsigned int top;
  int duty;
  int _dutyVal;
  int prescaler;
};
#endif


