#ifndef Timer
#define Timer

#include "atmegax8.h"

#define NUM_STATES 6

#define RAMP_INIT_FREQUENCY_T1 60000
#define RAMP_INIT_DUTY_T0 90
#define RAMP_INIT_REFREASHRATE 1

#define RAMP_FIN_FREQUENCY_T1 30000
#define RAMP_FIN_DUTY_T0 80
#define RAMP_FIN_REFREASHRATE 1

//#define DEFAULT_INITIAL_FREQ 40500
//#define DEFAULT_INITIAL_DUTY 80
//#define DEFAULT_INITIAL_PRESCALER 0

#define T0_FREQUENCY 200
#define T1_DUTY 1

#define DEFAULT_T1_INITIAL_PRESCALER 0
#define DEFAULT_T0_INITIAL_PRESCALER 0

class timer {

public:

	virtual int start() = 0;
	virtual int start(int prescaler) = 0;
	virtual int setFrequency(unsigned int val) = 0;
	virtual int setDuty(int val) = 0;
	virtual int setPrescaler(int val) = 0;
	virtual int getFrequency() = 0;
	virtual int getDuty() = 0;
	virtual int getPrescaler() = 0;
	//int stop(); no working with prescaler

private:

	
};
#endif
