#include "avrTimer.h"

// convert to TOP value for calculating register value
uint16_t
fastPWM_Hz2Top (uint16_t prescaler, uint32_t Hz)
{
  // debug("%lu,%u,%u",F_CPU,prescaler,Hz);
  return floor ((F_CPU / prescaler / Hz) - 1);
}

// used for calculating max TOP, min frequency
uint32_t
fastPWM_Top2Hz (uint16_t prescaler, uint16_t top)
{
  // debug("%lu,%u,%u", F_CPU, prescaler, top);
  return F_CPU / prescaler / (top + 1);
}

// convert to TOP value for calculating register value
uint16_t
phaseCorrectPWM_Hz2Top (uint16_t prescaler, uint32_t Hz)
{
  // debug("%lu,%u,%u",F_CPU,prescaler,Hz);
  return floor (F_CPU / prescaler / Hz / 2);
}

// used for calculating max TOP, min frequency
uint32_t
phaseCorrectPWM_Top2Hz (uint16_t prescaler, uint16_t top)
{
  // debug("%lu,%u,%u", F_CPU, prescaler, top);
  return F_CPU / prescaler / top / 2;
}

uint16_t
prescalerMinHz (timer_mode _mode, uint16_t _prescaler, uint8_t _timerSize)
{

  if (_mode == FASTPWM_TOP || _mode == FASTPWM_OCRA)
    {
      if (_timerSize == 16)
        {
          return fastPWM_Top2Hz (_prescaler, UINT16_MAX) + 1;
        }
      if (_timerSize == 8)
        {
          return fastPWM_Top2Hz (_prescaler, UINT8_MAX) + 1;
        }
    }

  if (_mode == PHASE_CORRECT_OCRA || PHASE_CORRECT_TOP)
    {
      if (_timerSize == 16)
        {
          return phaseCorrectPWM_Top2Hz (_prescaler, UINT16_MAX) + 1;
        }
      if (_timerSize == 8)
        {
          return phaseCorrectPWM_Top2Hz (_prescaler, UINT8_MAX) + 1;
        }
    }
}

long
avrMap (uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min,
        uint16_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
