#include "avrTimerFuncs.h"

//convert to TOP value for calculating register value
uint16_t fastPWM_Hz2Top(uint16_t prescaler, uint16_t Hz) {
  //debug("%lu,%u,%u",F_CPU,prescaler,Hz);
  return floor(F_CPU / (prescaler * Hz) - 1);
}

// used for calculating max TOP, min frequency
uint16_t fastPWM_Top2Hz(uint16_t prescaler, uint16_t top) {
  //debug("%lu,%u,%u", F_CPU, prescaler, top);
  return F_CPU /  (top + 1) / prescaler;
}

//convert to TOP value for calculating register value
uint8_t phaseCorrectPWM_Hz2Top(uint8_t prescaler, uint8_t Hz) {
  //debug("%lu,%u,%u",F_CPU,prescaler,Hz);
  return floor(F_CPU / Hz / prescaler);
}

// used for calculating max TOP, min frequency
uint8_t phaseCorrectPWM_Top2Hz(uint8_t prescaler, uint8_t top) {
  //debug("%lu,%u,%u", F_CPU, prescaler, top);
  return F_CPU / top / prescaler;
}


