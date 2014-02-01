#ifndef Timer_h
#define Timer_h

#include "timer.h"

timer::timer() {

	frequency = DEFAULT_INITIAL_FREQ;
	setDuty(DEFAULT_INITIAL_DUTY);
  prescaler = 0;
	_timer1_fastPwm_ocr1atop_init();
 
}

int timer::_timer1_fastPwm_icr1top_init() {
  
  SET_TIMER1_PINB;
	SET_TIMER1_FREQUENCY_ICR1TOP(frequency);
	SET_TIMER1_DUTY_CHAN_B(_dutyVal);
	TIMER1_RESET;
	SET_TIMER1_PINOUT(B);  
  SET_TIMER1_MODE_FASTPWM_ICR1;
	SET_TIMER1_PINB_NOTINVERTING(1);
	SET_TIMER1_INTERRUPT_OUTPUTCOMPARE_B;
  SET_TIMER1_PRESCALER_0;

 return 0;
}


int timer::_timer1_fastPwm_ocr1atop_init() {

  SET_TIMER1_PINB;
  SET_TIMER1_FREQUENCY_OCR1ATOP(frequency);
	SET_TIMER1_DUTY_CHAN_B(_dutyVal);
  TIMER1_RESET;
  SET_TIMER1_PINOUT(B);
	SET_TIMER1_MODE_FASTPWM_OCR1A;
	SET_TIMER1_PINB_NOTINVERTING(0);
	SET_TIMER1_INTERRUPT_OUTPUTCOMPARE_B;

  return 0;
}

int timer::start() {

	setPrescaler(prescaler);
	return 0;
}
int timer::start(int _prescaler) {
	setPrescaler(_prescaler);
  return 0;
}

//int timer::stop(){ //not working with prescaler
//	SET_TIMER1_STOP;
//}
int timer::setPrescaler(int _prescaler){
	
  switch(_prescaler) {
		case 0:
			SET_TIMER1_PRESCALER_0;
			prescaler = 0;
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

int timer::getPrescaler(){return prescaler;}

int timer::setFrequency(int val) {

	if (val == frequency) {
		return -1;
	}

	int zDuty = -10;

	if (frequency > val) {

		zDuty = setDuty(duty); 
		frequency = val;       
    SET_TIMER1_FREQUENCY_OCR1ATOP(frequency);
	}

	if (frequency < val) {
		frequency = val;       
    zDuty = setDuty(duty);
    SET_TIMER1_FREQUENCY_OCR1ATOP(frequency);
	}
	
	return zDuty;
}

int timer::setDuty(int val) {

	if (val < 0 || val >= 100)
		return -1;

	duty = val;
	_dutyVal = map(duty, 0, 100, 0, frequency);
  SET_TIMER1_DUTY_CHAN_B(_dutyVal);
	return 0;

}

int timer::getFrequency() {
	return frequency;
}
int timer::getDuty() {
	return duty;
}


void timer::_timer1_ovf_handler(){
	  SET_TIMER1_FREQUENCY_OCR1ATOP(frequency);
	  SET_TIMER1_DUTY_CHAN_B(_dutyVal);
}

#endif

