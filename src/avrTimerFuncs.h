#ifndef timerfuncs_h
#define timerfuncs_h

#include "stdint.h"
#include "math.h"

uint16_t fastPWM_Hz2Top(uint16_t prescaler, uint16_t Hz);
uint16_t fastPWM_Top2Hz(uint16_t prescaler, uint16_t top);
uint8_t phaseCorrectPWM_Hz2Top(uint8_t prescaler, uint8_t );
uint8_t phaseCorrectPWM_Top2Hz(uint8_t prescaler, uint8_t top);

#endif
