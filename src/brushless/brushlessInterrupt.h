#ifndef BRUSHLESS_INTERRUPT_H
#define BRUSHLESS_INTERRUPT_H

#include <avrTimer1.h>

// #include <logger.h>

// pointers
extern void (*myfunc) (void);

// functions
void registerISRCallback (void (*func) (void));
void startISR (uint32_t Hz);
void stopISR (void);
void setISRFrequency (uint32_t Hz);
uint16_t getISRFrequency (void);

#endif // header brushlessinterrupt_h
