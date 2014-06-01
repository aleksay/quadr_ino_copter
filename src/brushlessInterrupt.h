#ifndef brushlessinterrupt_h
#define brushlessinterrupt_h

#include "avrTimer1.h"
#include "stdio.h"

// pointers
void (*myfunc);

// functions
extern void registerISRCallback (void (*func)(void) );
void deregisterISRCallback ( void (*func)(void) );
void scheduleISRCallback (uint16_t Hz);

#endif //header scheduler_h
