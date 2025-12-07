#ifndef AVRTIMER0_H
#define AVRTIMER0_H

#include <avrTimer.h>


// Timer0 Reset

#define TIMER0_RESET                                                           \
  TCCR0B = 0;                                                                  \
  TCCR0A = 0;                                                                  \
  TCNT0 = 0;                                                                   \
  TIMSK0 = 0;                                                                  \
  TIFR0 = 0

#define TIMER_REGISTER_SIZE 8

// Timer0 Prescaler

/*
0 0 0 No clock source (Timer/Counter stopped).
0 0 1 clkT2S/(no prescaling)
0 1 0 clkT2S/8 (from prescaler)
0 1 1 clkT2S/64 (from prescaler)
1 0 0 clkT2S/256 (from prescaler)
1 0 1 clkT2S/1024 (from prescaler)
1 1 0 external (from prescaler)
1 1 1 external (from prescaler)
 */

#define SET_TIMER0_PRESCALER_0                                                 \
  TCCR0B &= ~((1 << CS02) | (1 << CS01) | (1 << CS00))
// #define SET_TIMER0_PRESCALER_1    TCCR0B |= (0 << CS02) | (0 << CS01) | (1 <<
// CS00)
#define SET_TIMER0_PRESCALER_1                                                 \
  TCCR0B &= ~((1 << CS02) | (1 << CS01));                                      \
  TCCR0B |= (1 << CS00)
// #define SET_TIMER0_PRESCALER_8    TCCR0B |= (0 << CS02) | (1 << CS01) | (0 <<
// CS00)
#define SET_TIMER0_PRESCALER_8                                                 \
  TCCR0B &= ~((1 << CS02) | (1 << CS00));                                      \
  TCCR0B |= (1 << CS01)
// #define SET_TIMER0_PRESCALER_32   TCCR0B |= (0 << CS02) | (1 << CS01) | (1 <<
// CS00)
#define SET_TIMER0_PRESCALER_64                                                \
  TCCR0B &= ~(1 << CS02);                                                      \
  TCCR0B |= (1 << CS01) | (1 << CS00)
// #define SET_TIMER0_PRESCALER_64   TCCR0B |= (1 << CS02) | (0 << CS01) | (0 <<
// CS00)
#define SET_TIMER0_PRESCALER_256                                               \
  TCCR0B |= (1 << CS02);                                                       \
  TCCR0B &= ~((1 << CS01) | (1 << CS00))
// #define SET_TIMER0_PRESCALER_128  TCCR0B |= (1 << CS02) | (0 << CS01) | (1 <<
// CS00)
#define SET_TIMER0_PRESCALER_1024                                              \
  TCCR0B |= (1 << CS02) | (1 << CS00);                                         \
  TCCR0B &= ~(1 << CS01)
// #define SET_TIMER0_PRESCALER_256  TCCR0B |= (1 << CS02) | (1 << CS01) | (0 <<
// CS00) #define SET_TIMER0_PRESCALER_256  TCCR0B |= (1 << CS02) | (1 <<
// CS01);TCCR0B &= ~(1 << CS00) #define SET_TIMER0_PRESCALER_1024 TCCR0B |= (1
// << CS02) | (1 << CS01) | (1 << CS00)


//   Timer0 Mode


/*

Mode | WGM2 | WGM1 | WGM0 | Timer/Counter Mode of Operation | TOP | Update of
OCRx at | TOV Flag Set on(1)(2) 0X     0      0      0           Normal 0xFF
Immediate             MAX 1X     0      0      1           PWM, phase correct
0xFF      TOP                BOTTOM 2X     0      1      0           CTC OCRA
Immediate             MAX 3X     0      1      1           Fast PWM 0xFF BOTTOM
MAX 4      1      0      0           Reserved – – – 5X     1      0      1 PWM,
phase correct         OCRA      TOP                BOTTOM 6      1      1      0
Reserved – – – 7X     1      1      1           Fast PWM                   OCRA
BOTTOM              TOP
*/

// #define SET_TIMER0_MODE_NORMAL                  TCCR0B |= (0 << WGM02);TCCR0A
// |= (0 << WGM01) | (0 << WGM00)
#define SET_TIMER0_MODE_NORMAL                                                 \
  TCCR0B &= ~(1 << WGM02);                                                     \
  TCCR0A &= ~((1 << WGM01) | (1 << WGM00))
// #define SET_TIMER0_MODE_PHASE_CORRECT_TOP       TCCR0B |= (0 << WGM02);TCCR0A
// |= (0 << WGM01) | (1 << WGM00)
#define SET_TIMER0_MODE_PHASE_CORRECT_TOP                                      \
  TCCR0B &= ~(1 << WGM02);                                                     \
  TCCR0A &= ~(1 << WGM01);                                                     \
  TCCR0A |= (1 << WGM00)
// #define SET_TIMER0_MODE_CTC_OCRA                TCCR0B |= (0 << WGM02);TCCR0A
// |= (1 << WGM01) | (0 << WGM00)
#define SET_TIMER0_MODE_CTC_OCRA                                               \
  TCCR0B &= ~(1 << WGM02);                                                     \
  TCCR0A |= (1 << WGM01);                                                      \
  TCCR0A &= ~(1 << WGM00)
// #define SET_TIMER0_MODE_FASTPWM_TOP             TCCR0B |= (0 << WGM02);TCCR0A
// |= (1 << WGM01) | (1 << WGM00)
#define SET_TIMER0_MODE_FASTPWM_TOP                                            \
  TCCR0B &= ~(1 << WGM02);                                                     \
  TCCR0A |= (1 << WGM01) | (1 << WGM00)
// #define SET_TIMER0_MODE_PHASE_CORRECT_OCRA      TCCR0B |= (1 << WGM02);TCCR0A
// |= (0 << WGM01) | (1 << WGM00)
#define SET_TIMER0_MODE_PHASE_CORRECT_OCRA                                     \
  TCCR0B |= (1 << WGM02);                                                      \
  TCCR0A &= ~(1 << WGM01);                                                     \
  TCCR0A |= (1 << WGM00)
#define SET_TIMER0_MODE_FASTPWM_OCRA                                           \
  TCCR0B |= (1 << WGM02);                                                      \
  TCCR0A |= (1 << WGM01) | (1 << WGM00)


#define SET_TIMER0_OCRA(val) OCR0A = val // TOP
#define SET_TIMER0_OCRB(val) OCR0B = val // DUTY

// Timer0 Pins

#define SET_TIMER0_PINA DDRD |= (1 << PD6) // digital pin 6
#define SET_TIMER0_PINB DDRD |= (1 << PD5) // digital pin  5

#define UNSET_TIMER0_PINA DDRD &= ~(1 << PD6) // digital pin 6
#define UNSET_TIMER0_PINB DDRD &= ~(1 << PD5) // digital pin  5

#define SET_TIMER0_PINA_CLEAR PORTD &= ~(1 << PD6)
#define SET_TIMER0_PINA_SET PORTD |= (1 << PD6)

#define SET_TIMER0_PINB_CLEAR PORTD &= ~(1 << PD5)
#define SET_TIMER0_PINB_SET PORTD |= (1 << PD5)

/*COM0A1/COM0B1 COM0A0/COM0B0*/
// #define SET_TIMER0_PINOUT(pin)   TCCR0A |= 1 << COM0##pin##1
// #define UNSET_TIMER0_PINOUT(pin) TCCR0A &= ~(1 << COM0##pin##1)
// #define SET_TIMER0_PINB_NOTINVERTING(notInverting) TCCR0A |= notInverting <<
// COM0B0 #define SET_TIMER0_PINA_NOTINVERTING(notInverting) TCCR0A |=
// notInverting << COM0A0

#define SET_TIMER0_PINA_DISABLE TCCR0A &= ~(1 << COM0A1) | ~(1 << COM0A0)
// COM0A1=0, COM0A0=1 per TIMER1 valido solo in NORMAL e CTC
#define SET_TIMER0_PINA_TOGGLE_ONCOMPARE                                       \
  TCCR0A &= ~(1 << COM0A1);                                                    \
  TCCR0A |= (1 << COM0A0)
// COM0A1=1, COM0A0=0
#define SET_TIMER0_PINA_CLEAR_ONCOMPARE                                        \
  TCCR0A |= (1 << COM0A1);                                                     \
  TCCR0A &= ~(1 << COM0A0)
// COM0A1=1, COM0A0=1
#define SET_TIMER0_PINA_SET_ONCOMPARE TCCR0A |= (1 << COM0A1) | (1 << COM0A0)

#define SET_TIMER0_PINB_DISABLE TCCR0A &= ~(1 << COM0B1) | ~(1 << COM0B0)
// COM0B1=0, COM0B0=1
#define SET_TIMER0_PINB_TOGGLE_ONCOMPARE                                       \
  TCCR0A &= ~(1 << COM0B1);                                                    \
  TCCR0A |= 1 << COM0B0
// COM0B1=1, COM0B0=0
#define SET_TIMER0_PINB_CLEAR_ONCOMPARE                                        \
  TCCR0A |= 1 << COM0B1;                                                       \
  TCCR0A &= ~(1 << COM0B0)
// COM0B1=1, COM0B0=1
#define SET_TIMER0_PINB_SET_ONCOMPARE TCCR0A |= (1 << COM0B1) | (1 << COM0B0)


// Timer0 Interrupts


#define SET_TIMER0_INTERRUPT_OUTPUTCOMPARE_A TIMSK0 |= (1 << OCIE0A)
#define SET_TIMER0_INTERRUPT_OUTPUTCOMPARE_B TIMSK0 |= (1 << OCIE0B)
#define SET_TIMER0_INTERRUPT_OVERFLOW TIMSK0 |= (1 << TOIE0)

#define UNSET_TIMER0_INTERRUPT_OUTPUTCOMPARE_A TIMSK0 &= ~(1 << OCIE0A)
#define UNSET_TIMER0_INTERRUPT_OUTPUTCOMPARE_B TIMSK0 &= ~(1 << OCIE0B)
#define UNSET_TIMER0_INTERRUPT_OVERFLOW TIMSK0 &= ~(1 << TOIE0)

extern void (*timer0_ovf_handler)();
extern void (*timer0_compa_handler)();
extern void (*timer0_compb_handler)();

// Var declarations
extern timer_mode timer0_mode;
extern uint16_t timer0_prescaler;
extern volatile uint8_t timer0_bottom;
extern volatile uint16_t timer0_top;
extern uint8_t timer0_duty;
extern uint8_t timer0_running;
extern uint16_t timer0_allowedPrescalers[];
extern uint8_t timer0_allowedPrescalersLenght;

// function declarations

void timer0_init(void);
void timer0_init(timer_mode _mode, uint16_t _prescaler);
void timer0_mode_normal_init(void);
void timer0_mode_phasecorrect_ocra_init(void);
void timer0_mode_ctc_ocra_init(void);
void timer0_mode_phasecorrect_top_init(void);
void timer0_mode_fastpwm_top_init(void);
void timer0_mode_fastpwm_ocra_init(void);

void timer0_setPin(char _pin, timer_pin_mode _mode);
void timer0_clearPin(char _pin);

void timer0_start(uint16_t _prescaler);
void timer0_start(void);
void timer0_stop(void);
void timer0_resetCounter(void);

void timer0_setTop(uint16_t _top);
void timer0_setBottom(uint8_t _bottom);
void timer0_setDuty(uint8_t _duty);
void timer0_setFrequency(uint32_t Hz);

void timer0_setPrescaler(uint16_t _prescaler);
void timer0_increasePrescaler(void);
void timer0_decreasePrescaler(void);

void timer0_register_COMPA_callback(void (*func)(void));
void timer0_register_COMPB_callback(void (*func)(void));
void timer0_register_OVF_callback(void (*func)(void));

unsigned long timer0_getOvfCounter(void);
uint16_t timer0_getTop(void);
uint8_t timer0_getDuty(void);
uint8_t timer0_isRunning(void);
uint16_t timer0_getNextPrescaler(void);
uint16_t timer0_getPrevPrescaler(void);
uint16_t timer0_getPrescaler(void);
uint32_t timer0_getFrequency(void);

#endif // header
