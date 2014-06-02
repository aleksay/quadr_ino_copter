#include "brushlessInterrupt.h"

// declare pointer
void (*myfunc)(void);



// functions
void registerISRCallback (  void(*func)(void) ){
myfunc=func;
timer1_start();
//timer1_start(int _prescaler);
}

void deregisterISRCallback (  void(*func)(void) ){
timer1_stop();
}


void scheduleISRCallback (uint16_t Hz){
timer1_setFrequency(Hz);
}

ISR(TIMER1_COMPA_vect) {
  (*myfunc);
}

