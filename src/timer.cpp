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

timer::timer() {

	frequency = DEFAULT_INITIAL_FREQ;
	setDuty(DEFAULT_INITIAL_DUTY);
	setPrescaler(DEFAULT_INITIAL_PRESCALER);

	_timer1_fastPwm_ocr1atop_init();
}

int timer::_timer1_fastPwm_ocr1atop_init() {

	SET_TIMER1_FREQUENCY_OCR1ATOP(frequency);
	SET_TIMER1_DUTY_CHAN_B(_dutyVal);
	SET_TIMER1_MODE_FASTPWM_OCR1A;
	SET_TIMER1_PINOUT(B, 1);
	SET_TIMER1_PRESCALER_8;

	SET_TIMER1_INTERRUPT_OUTPUTCOMPARE_B;
	SET_TIMER1_INTERRUPT_OVERFLOW;

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
	return 1;
}

}

int timer::getPrescaler(){return prescaler;}

int timer::setFrequency(int val) {

	if (val == frequency) {
		return -1;
	}

	int zDuty = -10;

	if (frequency > val) {

		zDuty = setDuty(duty); // to avoid duty out of range duty is decreased before the frequency 
		frequency = val;        // Assign the value set to frequency 
	}

	if (frequency < val) {
		frequency = val;        // Assign the value set to frequency
		zDuty = setDuty(duty);
	}
	
	return zDuty;
}

int timer::setDuty(int val) {

	if (val < 0 || val >= 100)
		return -1;

	duty = val;
	_dutyVal = map(duty, 0, 100, 0, frequency);

	return 0;

}

int timer::getFrequency() {
	return frequency;
}
int timer::getDuty() {
	return duty;
}

//ISR(TIMER1_OVF_vect){
void timer::_timer1_ovf_handler(){
		SET_TIMER1_FREQUENCY_OCR1ATOP(frequency); SET_TIMER1_DUTY_CHAN_B(_dutyVal);
}

#endif

