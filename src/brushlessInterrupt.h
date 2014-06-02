#ifndef brushlessinterrupt_h
#define brushlessinterrupt_h

#include "avrTimer1.h"
#include "stdio.h"

// pointers
extern void (*myfunc)(void);

// functions
void registerISRCallback ( void(*func)(void) );
void startISR (void );
void stopISR (void );
void setISRFrequency (uint16_t Hz);
uint16_t getISRFrequency (void);

#endif //header brushlessinterrupt_h
