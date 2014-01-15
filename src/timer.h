#ifndef Timer
#define Timer

#include "atmegax8.h"

#define DEFAULT_INITIAL_FREQ 800
#define DEFAULT_INITIAL_DUTY 50
#define DEFAULT_INITIAL_PRESCALER 256


class Timer {

public:

  //Timer(volatile int _frequency,int _duty, int _prescaler);
  //Timer(int _frequency,int _duty);
  Timer();
  int start();
  int start(int prescaler);
  setFrequency(int val);
  setDuty(int val);
  getFrequency();
  getDuty();
private:
  
  int _timer1_fastPwm_ocr1atop_init();

	

	int frequency;
  int duty;
	int _dutyVal;
	int prescaler;
};
#endif
