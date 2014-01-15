#ifndef Timer_h
#define Timer_h

#include "timer.h"

//  Timer::Timer(int _frequency,int _duty, int _prescaler){		

//		frequency  = _frequency;
//		setDuty(_duty);
 //   prescaler  = _prescaler;

//		_timer1_fastPwm_ocr1atop_init()  

//	}

//	Timer::Timer(int _frequency,int _duty){		
//		frequency  = _frequency;
//		setDuty(_duty);
 //   prescaler  = DEFAULT_INITIAL_PRESCALER;

//		_timer1_fastPwm_ocr1atop_init()  
 // }

  Timer::Timer()
{		

		frequency  = DEFAULT_INITIAL_FREQ;
		setDuty(DEFAULT_INITIAL_DUTY);
    prescaler  = DEFAULT_INITIAL_PRESCALER;

		_timer1_fastPwm_ocr1atop_init()
	}

	int Timer::_timer1_fastPwm_ocr1atop_init();
		
		SET_TIMER1_FREQUENCY_OCR1ATOP(frequency);
  	SET_TIMER1_DUTY_CHAN_B(_dutyVal);
  	SET_TIMER1_MODE_FASTPWM_OCR1A;
		SET_TIMER1_PINOUT(B,1);
   	SET_TIMER1_START(prescaler);

		SET_TIMER1_INTERRUPT_OUTPUTCOMPARE_B;
    SET_TIMER1_INTERRUPT_OVERFLOW;

  } 
 
	int Timer::start(){ SET_TIMER1_START(prescaler); }
	int Timer::start(int _prescaler){ SET_TIMER1_START(_prescaler); }

  int Timer::setFrequency(int val){
		
		if(val == frequency){ return -1; }

		if (frequency > val){
  		
  		setDuty(duty);          // to avoid duty out of range duty is decreased before the frequency 
  		frequency = val;        // Assign the value set to frequency 
		}

    if (frequency < val){
    	frequency = val;        // Assign the value set to frequency
    	setDuty(duty);
  	}
	}

  int Timer::setDuty(int val){
		
		if(val < 0 || val >= 100) return -1;

 		duty     = val;
  	_dutyVal = map(duty,0,100,0,frequency);		
	}

  int Timer::getFrequency(){ return frequency; }
  int Timer::getDuty(){ return duty;}
  

	ISR(TIMER1_OVF_vect){

		SET_TIMER1_FREQUENCY_OCR1ATOP(frequency);
		SET_TIMER1_DUTY_OCR1B(_dutyVal);
	}

	



#endif

