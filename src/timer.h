#ifndef Timer
#define Timer

#include "atmegax8.h"

#define DEFAULT_INITIAL_FREQ 800
#define DEFAULT_INITIAL_DUTY 50
#define DEFAULT_INITIAL_PRESCALER 256

class timer {

public:

	//Timer(volatile int _frequency,int _duty, int _prescaler);
	//Timer(int _frequency,int _duty);
	timer();
	int start();
	int start(int prescaler);
	int setFrequency(int val);
	int setDuty(int val);
  int setPrescaler(int val);
	int getFrequency();
	int getDuty();
	int getPrescaler();
private:

	int _timer1_fastPwm_ocr1atop_init();
  void _timer1_ovf_handler(); 

	int frequency;
	int duty;
	int _dutyVal;
	int prescaler;
};
#endif
