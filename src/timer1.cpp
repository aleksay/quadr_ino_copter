#ifndef Timer_h
#define Timer_h
#include "timer.h"
#include "communicator.h"

class timer1: 
public timer
{
public:
  timer1() {

		frequency = 0;
  	
	//initialize timer1 global variable
		setFrequency(DEFAULT_T1_INIT_FREQUENCY);
    setDuty(DEFAULT_T1_INIT_DUTY);
    
    //prescaler = DEFAULT_T1_INIT_PRESCALER;
	
	//configure timer1
    _timer1_fastPwm_ocr1atop_init();

  }

  int _timer1_fastPwm_icr1top_init() {

    SET_TIMER1_PINB;
    SET_TIMER1_FREQUENCY_ICR1TOP(frequency);

    SET_TIMER1_DUTY_CHAN_B(_dutyVal);

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
    SET_TIMER1_MODE_FASTPWM_OCR1A;
    SET_TIMER1_PINB_NOTINVERTING(0);

    return 0;
  }

  int start() {
    //debug(String("prescaler set to: ")+ prescaler,3);
    
    setPrescaler(prescaler);
    return 0;
  }
  int start(int _prescaler) {
    //debug(String("_prescaler set to: " + _prescaler) ,3);
    setPrescaler(_prescaler);
    return 0;
  }

  //int timer::stop(){ //not working with prescaler
  //	SET_TIMER1_STOP;
  //}

  int setPrescaler(int _prescaler){
    
    switch(_prescaler) {
    
    case 1:
      debug(_prescaler,3);
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
//         debug("funzione di conversione:",3);
//         debug(freqHz,3);
//         debug(F_CPU,3);
//         debug(getPrescaler(),3);
//         debug(floor(F_CPU/(getPrescaler() * freqHz)-1),3);
         
	 return floor(F_CPU/(getPrescaler() * freqHz)-1);
	}
  
 
	int setFrequency(unsigned int freqHz) {
		int _top=0;
		// check value
		if (freqHz == frequency) {
			return -1;
		}
		//update global variable frequency
		frequency = freqHz;
	

		//convert value to microcontroller TOP
		_top = Hz2top(freqHz);
		      //debug(_top,3);
	
		//check TOP consistency
		if (_top < 245 || _top > 65535){ //limiti x non bloccare il microcontrollore non avendo il controllo sul prescaler
			return -1;
		}
		top=_top;
		
//		if (_top == top) {
//			return -1;
//		}

		int zDuty = 1;
		       
		//set new value on the register
		SET_TIMER1_FREQUENCY_OCR1ATOP(top);
	
		zDuty = setDuty(duty);

	  return zDuty;
  }

  int setDuty(int val) {

    if (val < 0 || val > 100)
      return -1;

    duty = val;
    _dutyVal = map(duty, 0, 100, 0, top);
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


