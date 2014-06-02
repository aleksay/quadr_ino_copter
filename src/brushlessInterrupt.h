#ifndef brushlessinterrupt_h
#define brushlessinterrupt_h

#include "avrTimer1.h"
#include "stdio.h"

// pointers
extern void (*myfunc)(void);

// functions
void registerISRCallback ( void(*func)(void) );
void deregisterISRCallback ( void(*func)(void) );
void scheduleISRCallback (uint16_t Hz);

#endif //header brushlessinterrupt_h
