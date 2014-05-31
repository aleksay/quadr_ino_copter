#ifndef AVRTIMER0_H
#define AVRTIMER0_H 

#include "comLogger.h"
#include "config.h"
#include "avrUtils.h"

/*
=====================================================
 
 		           Timer 0 Section
 
 =====================================================
 */
#define TIMER0_RESET TCCR0B=0;TCCR0A=0;TCNT0=0

/*
0 0 0 No clock source (Timer/Counter stopped)
 0 0 1 clkI/O/1 (No prescaling)
 0 1 0 clkI/O/8 (From prescaler)
 0 1 1 clkI/O/64 (From prescaler)
 1 0 0 clkI/O/256 (From prescaler)
 1 0 1 clkI/O/1024 (From prescaler)
 1 1 0 External clock source on T0 pin. Clock on falling edge.
 1 1 1 External clock source on T0 pin. Clock on rising edge
 */

#define SET_TIMER0_STOP           TCCR0B &= ~((1 << CS02) | (1 << CS01)  | (1 << CS00))
#define SET_TIMER0_PRESCALER_1    TCCR0B |= (0 << CS02) | (0 << CS01) | (1 << CS00)
#define SET_TIMER0_PRESCALER_8    TCCR0B |= (0 << CS02) | (1 << CS01) | (0 << CS00)
#define SET_TIMER0_PRESCALER_64   TCCR0B |= (0 << CS02) | (1 << CS01) | (1 << CS00)
#define SET_TIMER0_PRESCALER_256  TCCR0B |= (1 << CS02) | (0 << CS01) | (0 << CS00)
#define SET_TIMER0_PRESCALER_1024 TCCR0B |= (1 << CS02) | (0 << CS01) | (1 << CS00)

/*
WGM02 WGM01 WGM00 Timer/Counter 
 1 0 1 PWM, Phase Correct OCRA TOP BOTTOM
 1 1 1 Fast PWM OCRA BOTTOM TOP
 */

#define SET_TIMER0_MODE_PHASE_CORRECT_OCR0A     TCCR0B |= (1 << WGM02);TCCR0A |= (0 << WGM01) | (1 << WGM00)
#define SET_TIMER0_MODE_FASTPWM_OCR0A           TCCR0B |= (1 << WGM02);TCCR0A |= (1 << WGM01) | (1 << WGM00)

#define SET_TIMER0_FREQUENCY(val) OCR0A = val
#define SET_TIMER0_DUTY(val)      OCR0B = val

#define SET_TIMER0_PINB DDRD = DDRD | 0b00100000
#define SET_TIMER0_PINA DDRD = DDRD | 0b01000000
/*COM1A1/COM1B1 COM1A0/COM1B0*/
#define SET_TIMER0_PINOUT(pin)   TCCR0A |= 1 << COM0##pin##1
#define UNSET_TIMER0_PINOUT(pin) TCCR0A &= ~(1 << COM0##pin##1) 

#define SET_TIMER0_PINB_NOTINVERTING(notInverting) TCCR0A |= notInverting << COM0B0
#define SET_TIMER0_PINA_NOTINVERTING(notInverting) TCCR0A |= notInverting << COM0A0

#define SET_TIMER0_INTERRUPT_OUTPUTCOMPARE_B TIMSK0 |= (1 << OCIE0B)
#define SET_TIMER0_INTERRUPT_OVERFLOW        TIMSK0 |= (1 << TOIE0 )







// FUNCTION DECLARATIONS
int timer0_init();
void timer0_phasecorrect_ocr0atop_init();
int timer0_start();
int timer0_start(int _prescaler) ;
int timer0_stop();
int timer0_setPrescaler(int _prescaler);
int timer0_getPrescaler();
int timer0_setFrequency(unsigned int val) ;
int timer0_setDuty(int val) ;
unsigned int timer0_getFrequency();
int timer0_getDuty();

#endif // header







