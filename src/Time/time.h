#ifndef TIME_H
#define TIME_H

#define _AVR_CLOCKTIMER_TIMER2_
//#define _AVR_CLOCKTIMER_TIMER0_

#ifdef __AVR__

#ifdef _AVR_CLOCKTIMER_TIMER2_
#include <avrTimer2.h>
#endif

#ifdef _AVR_CLOCKTIMER_TIMER0_
#include <avrTimer0.h>
#endif

#endif

extern volatile uint32_t seconds;
extern volatile uint32_t milliseconds;

void timeClock_init (void);
uint32_t timeClock_getMillis (void);
uint32_t timeClock_getSeconds (void);
void timeClock_millisCallback (void);
void timeClock_wait (uint32_t ms);

#endif // header include
