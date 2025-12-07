#ifndef AVRTIMER2_H
#define AVRTIMER2_H

#include <avrTimer.h>

/*
=====================================================

                Timer 2 Section

 =====================================================
 */
#define TIMER2_RESET                                                           \
  TCCR2B = 0;                                                                  \
  TCCR2A = 0;                                                                  \
  TCNT2 = 0;                                                                   \
  TIMSK2 = 0;                                                                  \
  TIFR2 = 0

#define TIMER_REGISTER_SIZE 8

/*
0 0 0 No clock source (Timer/Counter stopped).
0 0 1 clkT2S/(no prescaling)
0 1 0 clkT2S/8 (from prescaler)
0 1 1 clkT2S/32 (from prescaler)
1 0 0 clkT2S/64 (from prescaler)
1 0 1 clkT2S/128 (from prescaler)
1 1 0 clkT2S/256 (from prescaler)
1 1 1 clkT2S/1024 (from prescaler)
 */

#define SET_TIMER2_PRESCALER_0                                                 \
  TCCR2B &= ~((1 << CS22) | (1 << CS21) | (1 << CS20))
// #define SET_TIMER2_PRESCALER_1    TCCR2B |= (0 << CS22) | (0 << CS21) | (1 <<
// CS20)
#define SET_TIMER2_PRESCALER_1                                                 \
  TCCR2B &= ~((1 << CS22) | (1 << CS21));                                      \
  TCCR2B |= (1 << CS20)
// #define SET_TIMER2_PRESCALER_8    TCCR2B |= (0 << CS22) | (1 << CS21) | (0 <<
// CS20)
#define SET_TIMER2_PRESCALER_8                                                 \
  TCCR2B &= ~((1 << CS22) | (1 << CS20));                                      \
  TCCR2B |= (1 << CS21)
// #define SET_TIMER2_PRESCALER_32   TCCR2B |= (0 << CS22) | (1 << CS21) | (1 <<
// CS20)
#define SET_TIMER2_PRESCALER_32                                                \
  TCCR2B &= ~(1 << CS22);                                                      \
  TCCR2B |= (1 << CS21) | (1 << CS20)
// #define SET_TIMER2_PRESCALER_64   TCCR2B |= (1 << CS22) | (0 << CS21) | (0 <<
// CS20)
#define SET_TIMER2_PRESCALER_64                                                \
  TCCR2B |= (1 << CS22);                                                       \
  TCCR2B &= ~((1 << CS21) | (1 << CS20))
// #define SET_TIMER2_PRESCALER_128  TCCR2B |= (1 << CS22) | (0 << CS21) | (1 <<
// CS20)
#define SET_TIMER2_PRESCALER_128                                               \
  TCCR2B |= (1 << CS22) | (1 << CS20);                                         \
  TCCR2B &= ~(1 << CS21)
// #define SET_TIMER2_PRESCALER_256  TCCR2B |= (1 << CS22) | (1 << CS21) | (0 <<
// CS20)
#define SET_TIMER2_PRESCALER_256                                               \
  TCCR2B |= (1 << CS22) | (1 << CS21);                                         \
  TCCR2B &= ~(1 << CS20)
#define SET_TIMER2_PRESCALER_1024                                              \
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20)

/*

Mode WGM2 WGM1 WGM0
Timer/Counter Mode of
Operation TOP
Update of
OCRx at
TOV Flag
Set on(1)(2)
0 0 0 0 Normal 0xFF Immediate MAX
1 0 0 1 PWM, phase correct 0xFF TOP BOTTOM
2 0 1 0 CTC OCRA Immediate MAX
3 0 1 1 Fast PWM 0xFF BOTTOM MAX
4 1 0 0 Reserved – – –
5 1 0 1 PWM, phase correct OCRA TOP BOTTOM
6 1 1 0 Reserved – – –
7 1 1 1 Fast PWM OCRA BOTTOM TOP
*/

// #define SET_TIMER2_MODE_NORMAL                 TCCR2B |= (0 << WGM22);TCCR2A
// |= (0 << WGM21) | (0 << WGM20)
#define SET_TIMER2_MODE_NORMAL                                                 \
  TCCR2B &= ~(1 << WGM22);                                                     \
  TCCR2A &= ~((1 << WGM21) | (1 << WGM20))
// #define SET_TIMER2_MODE_PHASE_CORRECT_TOP       TCCR2B |= (0 << WGM22);TCCR2A
// |= (0 << WGM21) | (1 << WGM20)
#define SET_TIMER2_MODE_PHASE_CORRECT_TOP                                      \
  TCCR2B &= ~(1 << WGM22);                                                     \
  TCCR2A &= ~(1 << WGM21);                                                     \
  TCCR2A |= (1 << WGM20)
// #define SET_TIMER2_MODE_CTC_OCRA                TCCR2B |= (0 << WGM22);TCCR2A
// |= (1 << WGM21) | (0 << WGM20)
#define SET_TIMER2_MODE_CTC_OCRA                                               \
  TCCR2B &= ~(1 << WGM22);                                                     \
  TCCR2A |= (1 << WGM21);                                                      \
  TCCR2A &= ~(1 << WGM20)
// #define SET_TIMER2_MODE_FASTPWM_TOP             TCCR2B |= (0 << WGM22);TCCR2A
// |= (1 << WGM21) | (1 << WGM20)
#define SET_TIMER2_MODE_FASTPWM_TOP                                            \
  TCCR2B &= ~(1 << WGM22);                                                     \
  TCCR2A |= (1 << WGM21) | (1 << WGM20)
// #define SET_TIMER2_MODE_PHASE_CORRECT_OCRA      TCCR2B |= (1 << WGM22);TCCR2A
// |= (0 << WGM21) | (1 << WGM20)
#define SET_TIMER2_MODE_PHASE_CORRECT_OCRA                                     \
  TCCR2B |= (1 << WGM22);                                                      \
  TCCR2A &= ~(1 << WGM21);                                                     \
  TCCR2A |= (1 << WGM20)
#define SET_TIMER2_MODE_FASTPWM_OCRA                                           \
  TCCR2B |= (1 << WGM22);                                                      \
  TCCR2A |= (1 << WGM21) | (1 << WGM20)

#define SET_TIMER2_OCRA(val) OCR2A = val // TOP
#define SET_TIMER2_OCRB(val) OCR2B = val // DUTY

#define SET_TIMER2_PINA DDRB |= (1 << PB3) // digital pin 11
#define SET_TIMER2_PINB DDRD |= (1 << PD3) // digital pin  3

#define UNSET_TIMER2_PINA DDRB &= ~(1 << PB3) // digital pin 11
#define UNSET_TIMER2_PINB DDRD &= ~(1 << PD3) // digital pin  3

#define SET_TIMER2_PINA_CLEAR PORTB &= ~(1 << PB3)
#define SET_TIMER2_PINA_SET PORTB |= (1 << PB3)

#define SET_TIMER2_PINB_CLEAR PORTD &= ~(1 << PD3)
#define SET_TIMER2_PINB_SET PORTD |= (1 << PD3)

/*COM2A1/COM2B1 COM2A0/COM2B0*/
// #define SET_TIMER2_PINOUT(pin)   TCCR2A |= 1 << COM2##pin##1
// #define UNSET_TIMER2_PINOUT(pin) TCCR2A &= ~(1 << COM2##pin##1)
// #define SET_TIMER2_PINB_NOTINVERTING(notInverting) TCCR2A |= notInverting <<
// COM2B0 #define SET_TIMER2_PINA_NOTINVERTING(notInverting) TCCR2A |=
// notInverting << COM2A0

#define SET_TIMER2_PINA_DISABLE TCCR2A &= ~(1 << COM2A1) | ~(1 << COM2A0)
// COM2A1=0, COM2A0=1 per TIMER1 valido solo in NORMAL e CTC
#define SET_TIMER2_PINA_TOGGLE_ONCOMPARE                                       \
  TCCR2A &= ~(1 << COM2A1);                                                    \
  TCCR2A |= 1 << COM2A0
// COM2A1=1, COM2A0=0
#define SET_TIMER2_PINA_CLEAR_ONCOMPARE                                        \
  TCCR2A |= 1 << COM2A1;                                                       \
  TCCR2A &= ~(1 << COM2A0)
// COM2A1=1, COM2A0=1
#define SET_TIMER2_PINA_SET_ONCOMPARE TCCR2A |= (1 << COM2A1) | (1 << COM2A0)

#define SET_TIMER2_PINB_DISABLE TCCR2A &= ~(1 << COM2B1) | ~(1 << COM2B0)
// COM2B1=0, COM2B0=1
#define SET_TIMER2_PINB_TOGGLE_ONCOMPARE                                       \
  TCCR2A &= ~(1 << COM2B1);                                                    \
  TCCR2A |= 1 << COM2B0
// COM2B1=1, COM2B0=0
#define SET_TIMER2_PINB_CLEAR_ONCOMPARE                                        \
  TCCR2A |= 1 << COM2B1;                                                       \
  TCCR2A &= ~(1 << COM2B0)
// COM2B1=1, COM2B0=1
#define SET_TIMER2_PINB_SET_ONCOMPARE TCCR2A |= (1 << COM2B1) | (1 << COM2B0)

#define SET_TIMER2_INTERRUPT_OUTPUTCOMPARE_A TIMSK2 |= (1 << OCIE2A)
#define SET_TIMER2_INTERRUPT_OUTPUTCOMPARE_B TIMSK2 |= (1 << OCIE2B)
#define SET_TIMER2_INTERRUPT_OVERFLOW TIMSK2 |= (1 << TOIE2)

#define UNSET_TIMER2_INTERRUPT_OUTPUTCOMPARE_A TIMSK2 &= ~(1 << OCIE2A)
#define UNSET_TIMER2_INTERRUPT_OUTPUTCOMPARE_B TIMSK2 &= ~(1 << OCIE2B)
#define UNSET_TIMER2_INTERRUPT_OVERFLOW TIMSK2 &= ~(1 << TOIE2)

#define SET_TIMER2_CLOCK_INTERNAL ASSR &= ~(1 << AS2)
#define SET_TIMER2_CLOCK_TOSC1 ASSR |= (1 << AS2)

extern void (*timer2_ovf_handler)();
extern void (*timer2_compa_handler)();
extern void (*timer2_compb_handler)();

extern timer_mode timer2_mode;
extern uint16_t timer2_prescaler;
extern volatile uint8_t timer2_bottom;
extern volatile uint16_t timer2_top;
extern uint8_t timer2_duty;
extern uint8_t timer2_running;
extern uint16_t timer2_allowedPrescalers[];
extern uint8_t timer2_allowedPrescalersLenght;

// function declarations

void timer2_init(void);
void timer2_init(timer_mode mode, uint16_t _prescaler);
void timer2_mode_normal_init(void);
void timer2_mode_phasecorrect_ocra_init(void);
void timer2_mode_ctc_ocra_init(void);
void timer2_mode_phasecorrect_top_init(void);
void timer2_mode_fastpwm_top_init(void);
void timer2_mode_fastpwm_ocra_init(void);
void timer2_setPin(char _pin, timer_pin_mode _mode);
void timer2_clearPin(char _pin);

void timer2_start(uint16_t _prescaler);
void timer2_start(void);
void timer2_stop(void);
void timer2_resetCounter(void);

void timer2_setTop(uint16_t _top);
void timer2_setBottom(uint8_t _bottom);
void timer2_setDuty(uint8_t _duty);
void timer2_setFrequency(uint32_t Hz);

void timer2_setPrescaler(uint16_t _prescaler);
void timer2_increasePrescaler(void);
void timer2_decreasePrescaler(void);

void timer2_register_COMPA_callback(void (*func)(void));
void timer2_register_COMPB_callback(void (*func)(void));
void timer2_register_OVF_callback(void (*func)(void));

unsigned long timer2_getOvfCounter(void);
uint16_t timer2_getTop(void);
uint8_t timer2_getDuty(void);
uint8_t timer2_isRunning(void);
uint16_t timer2_getNextPrescaler(void);
uint16_t timer2_getPrevPrescaler(void);
uint16_t timer2_getPrescaler(void);
uint32_t timer2_getFrequency(void);

#endif // header
