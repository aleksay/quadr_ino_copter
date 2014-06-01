#include "scheduler.h"

// declare pointer
void (*myfunc)();



// functions
void registerISRCallback ( *func(void) ){
myfunc=func;
timer1_start();
//timer1_start(int _prescaler);
}

void deregisterISRCallback ( *func(void) ){
timer1_stop();
}


void scheduleISRCallback (unsigned int freqHz){
timer1_setFrequency(unsigned int freqHz);
}

ISR(TIMER1_COMPA_vect) {
  myfunc();
}

