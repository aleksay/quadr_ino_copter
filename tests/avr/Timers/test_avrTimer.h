#ifndef TESTAVRTIMER_H
#define TESTAVRTIMER_H

#include <avrTimer.h>

uint8_t test_fastPWM_Hz2Top(uint16_t prescaler, uint16_t Hz);
uint8_t test_fastPWM_Top2Hz(uint16_t prescaler, uint16_t top);
uint8_t test_phaseCorrectPWM_Hz2Top(uint16_t prescaler, uint16_t Hz);
uint8_t test_phaseCorrectPWM_Top2Hz(uint16_t prescaler, uint8_t top);

uint8_t test_avrMap(long x, long in_min, long in_max, long out_min,
                    long out_max);

#endif
