#ifndef AVRTIMER_h
#define AVRTIMER_h

#include <math.h>
#include <stdint.h>

#include <avr/interrupt.h>
#include <avr/io.h>


#ifndef F_CPU
#define F_CPU 16000000UL
#endif

/*
Timer Modes
                        Timer0	Timer1	Timer2
NORMAL			x	x       x
PHASE_CORRECT_TOP	x	x       x
CTC_OCRA		x	x	x
FASTPWM_TOP		x	x       x
PHASE_CORRECT_OCRA	x	x	x
FASTPWM_OCRA		x	x	x
PHASEFREQ_CORRECT_ICR		x
PHASEFREQ_CORRECT_OCRA		x
PHASE_CORRECT_ICR		x
CTC_ICR				x
FASTPWM_ICR			x
PHASE_CORRECT_TOP_9BIT,
PHASE_CORRECT_TOP_10BIT         x
FASTPWM_TOP_9BIT      ,
FASTPWM_TOP_10BIT               x 


*/

enum timer_mode {
  NORMAL,
  PHASE_CORRECT_TOP,
  CTC_OCRA,
  FASTPWM_TOP,
  PHASE_CORRECT_OCRA,
  FASTPWM_OCRA,
  PHASEFREQ_CORRECT_ICR,
  PHASEFREQ_CORRECT_OCRA,
  PHASE_CORRECT_ICR,
  CTC_ICR,
  FASTPWM_ICR,
  PHASE_CORRECT_TOP_9BIT,
  PHASE_CORRECT_TOP_10BIT,
  FASTPWM_TOP_9BIT,
  FASTPWM_TOP_10BIT               
};

enum timer_pin_mode { TOGGLE, CLEAR, SET };

/*
enum timer_prescaler
{
 TIMER_PRESCALER_0,
 TIMER_PRESCALER_1,
 TIMER_PRESCALER_8,
 TIMER_PRESCALER_32,
 TIMER_PRESCALER_64,
 TIMER_PRESCALER_128,
 TIMER_PRESCALER_256,
 TIMER_PRESCALER_1024
};
*/

uint16_t fastPWM_Hz2Top(uint16_t prescaler, uint32_t Hz);
uint32_t fastPWM_Top2Hz(uint16_t prescaler, uint16_t top);
uint16_t phaseCorrectPWM_Hz2Top(uint16_t prescaler, uint32_t Hz);
uint32_t phaseCorrectPWM_Top2Hz(uint16_t prescaler, uint16_t top);
uint16_t prescalerMinHz(timer_mode _mode, uint16_t _prescaler,
                        uint8_t _timerSize);

long avrMap(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);

#endif
