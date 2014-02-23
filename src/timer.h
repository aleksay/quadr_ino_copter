#ifndef Timer
#define Timer

#include "atmegax8.h"


#define DEFAULT_INITIAL_FREQ 500
#define DEFAULT_INITIAL_DUTY 80
#define DEFAULT_INITIAL_PRESCALER 0

#define T0_FREQUENCY 200
#define T0_DUTY 5
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
