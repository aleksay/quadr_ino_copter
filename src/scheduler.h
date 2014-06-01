#ifndef scheduler_h
#define scheduler_h

#include "avrTimer1.h"

// pointers
void (*myfunc);

// functions
void registerISRCallback ( *func(void) );
void deregisterISRCallback ( *func(void) );
void scheduleISRCallback (unsigned int freqHz);

#endif //header scheduler_h
