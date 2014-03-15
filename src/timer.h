#ifndef Timer
#define Timer

#include "atmegax8.h"



#define DEFAULT_T1_INIT_FREQUENCY 65535
#define DEFAULT_T1_INIT_DUTY 1
#define DEFAULT_T1_INIT_PRESCALER 0

#define DEFAULT_T0_INIT_FREQUENCY 200
#define DEFAULT_T0_INIT_DUTY 99

#define DEFAULT_T0_INIT_PRESCALER 0

class timer {

public:

	virtual int start() = 0;
	virtual int start(int prescaler) = 0;
	virtual int setFrequency(unsigned int val) = 0;
	virtual int setDuty(int val) = 0;
	virtual int setPrescaler(int val) = 0;
	virtual unsigned int getFrequency() = 0;
	virtual int getDuty() = 0;
	virtual int getPrescaler() = 0;
	//int stop(); no working with prescaler

private:

	
};
#endif
