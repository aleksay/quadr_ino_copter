#include "brushlessInterrupt.h"

// declare pointer
void (*myfunc)(void);



// functions
void registerISRCallback (  void(*func)(void) ){
debug("registering timer 1 ISR");
timer1_init();
myfunc=func;
}


// functions
void startISR (  void ){
debug("starting timer 1 ISR");
timer1_start();
//timer1_start(int _prescaler);
}

void stopISR ( void ){
debug("starting timer 1 ISR");
timer1_stop();
}


void setISRFrequency (uint16_t Hz){
timer1_setFrequency(Hz);
}

uint16_t getISRFrequency (void){
return timer1_getFrequency();
}

ISR(TIMER1_COMPA_vect) {
debug("ISR");
  (*myfunc);
}

