#ifndef AVRTIMER1_H
#define AVRTIMER1_H

#include <avrTimer.h>

//  TCNT1, OCR1A/B, and ICR1 are 16-bit registers

// Timer1 Reset

#define TIMER1_RESET                                                          \
  TCCR1B = 0;                                                                 \
  TCCR1A = 0;                                                                 \
  TCNT1 = 0;                                                                  \
  TIMSK1 = 0;                                                                 \
  TIFR1 = 0

// Timer1 Prescaler

/*
 CS12 CS11 CS10 Description
 0 0 0 No clock source (Timer/Counter stopped).
 0 0 1 clkI/O/0 (No prescaling)
 0 1 0 clkI/O/8 (From prescaler)
 0 1 1 clkI/O/64 (From prescaler)
 1 0 0 clkI/O/256 (From prescaler)
 1 0 1 clkI/O/1024 (From prescaler)
 */
#define SET_TIMER1_PRESCALER_0                                                \
  TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10))
#define SET_TIMER1_PRESCALER_1                                                \
  TCCR1B &= ~((1 << CS12) | (1 << CS11));                                     \
  TCCR1B |= (1 << CS10)
#define SET_TIMER1_PRESCALER_8                                                \
  TCCR1B &= ~((1 << CS12) | (1 << CS10));                                     \
  TCCR1B |= (1 << CS11)
#define SET_TIMER1_PRESCALER_64                                               \
  TCCR1B &= ~(1 << CS12);                                                     \
  TCCR1B |= (1 << CS11) | (1 << CS10)
#define SET_TIMER1_PRESCALER_256                                              \
  TCCR1B &= ~((1 << CS11) | (1 << CS10));                                     \
  TCCR1B |= (1 << CS12)
#define SET_TIMER1_PRESCALER_1024                                             \
  TCCR1B &= ~(1 << CS11);                                                     \
  TCCR1B |= (1 << CS12) | (1 << CS10)

// Timer1 mode

/*
Mode WGM13 WGM12 WGM11 WGM10 | Timer/Counter Mode ofOperation | TOP | Update of
OCR1x at | TOV1 Flag Set on 0       0    0     0     0           Normal 0xFFFF
Immediate        MAX 1       0    0     0     1     PWM, phase correct, 8-bit
0x00FF          TOP           BOTTOM 2 0 0 1 0                      PWM, phase
correct, 9-bit       0x01FF          TOP           BOTTOM 3 0 0 1 1 PWM, phase
correct, 10-bit      0x03FF          TOP           BOTTOM 4 0 1 0 0 CTC OCR1A
Immediate        MAX 5 0 1 0 1 Fast PWM, 8-bit 0x00FF BOTTOM TOP 6 0 1 1 0 Fast
PWM, 9-bit 0x01FF BOTTOM TOP 7 0 1 1 1 Fast PWM, 10-bit 0x03FF BOTTOM TOP 8 1 0
0 0 PWM, phase and frequency correct ICR1 BOTTOM BOTTOM 9 1 0 0 1 PWM, phase
and frequency correct OCR1A BOTTOM BOTTOM 10 1 0 1 0 PWM, phase correct ICR1
TOP BOTTOM 11 1 0 1 1 PWM, phase correct OCR1A TOP BOTTOM 12 1 1 0 0 CTC ICR1
Immediate MAX 14 1 1 1 0 Fast PWM ICR1 BOTTOM TOP 15 1 1 1 1 Fast PWM OCR1A
BOTTOM TOP

// compatible modes:
NORMAL,
PHASE_CORRECT_TOP,
CTC_OCRA,
FASTPWM_TOP,
PHASE_CORRECT_OCRA,
FASTPWM_OCRA,

// dedicated modes:
  PHASEFREQ_CORRECT_ICR,
  PHASEFREQ_CORRECT_OCRA,
  PHASE_CORRECT_ICR,
  CTC_ICR,
  FASTPWM_ICR,
  PHASE_CORRECT_TOP_9BIT,
  PHASE_CORRECT_TOP_10BIT,
  FASTPWM_TOP_9BIT,
  FASTPWM_TOP_10BIT

 */

// #define SET_TIMER1_MODE_NORMAL TCCR1B |= (0 << WGM13) | (0 << WGM12);TCCR1A
// |= (0 << WGM11) | (0 << WGM10)
#define SET_TIMER1_MODE_NORMAL                                                \
  TCCR1B &= ~((1 << WGM13) | (1 << WGM12));                                   \
  TCCR1A &= ~((1 << WGM11) | (1 << WGM10))
// #define SET_TIMER1_MODE_PHASE_CORRECT_TOP TCCR1B |= (0 << WGM13) | (0 <<
// WGM12);TCCR1A |= (0 << WGM11) | (1 << WGM10)
#define SET_TIMER1_MODE_PHASE_CORRECT_TOP                                     \
  TCCR1B &= ~((1 << WGM13) | (1 << WGM12));                                   \
  TCCR1A &= ~(1 << WGM11);                                                    \
  TCCR1A |= (1 << WGM10)
// #define SET_TIMER1_MODE_CTC_OCRA TCCR1B |= (0 << WGM13) | (1 <<
// WGM12);TCCR1A |= (0 << WGM11) | (0 << WGM10)
#define SET_TIMER1_MODE_CTC_OCRA                                              \
  TCCR1B &= ~(1 << WGM13);                                                    \
  TCCR1B |= (1 << WGM12);                                                     \
  TCCR1A &= ~((1 << WGM11) | (1 << WGM10))
// 0 1 0 1
#define SET_TIMER1_MODE_FASTPWM_TOP                                           \
  TCCR1B &= ~(1 << WGM13);                                                    \
  TCCR1B |= (1 << WGM12);                                                     \
  TCCR1A &= ~(1 << WGM11);                                                    \
  TCCR1A |= (1 << WGM10)
// #define SET_TIMER1_MODE_PHASE_CORRECT_OCRA TCCR1B |= (1 << WGM13) | (0 <<
// WGM12);TCCR1A |= (1 << WGM11) | (1 << WGM10)
#define SET_TIMER1_MODE_PHASE_CORRECT_OCRA                                    \
  TCCR1B |= (1 << WGM13);                                                     \
  TCCR1B &= ~(1 << WGM12);                                                    \
  TCCR1A |= (1 << WGM11) | (1 << WGM10)

#define SET_TIMER1_MODE_FASTPWM_OCRA                                          \
  TCCR1B |= (1 << WGM13) | (1 << WGM12);                                      \
  TCCR1A |= (1 << WGM11) | (1 << WGM10)

// #define SET_TIMER1_MODE_PHASEFREQ_CORRECT_ICR TCCR1B |= (1 << WGM13) | (0 <<
// WGM12);TCCR1A |= (0 << WGM11) | (0 << WGM10)
#define SET_TIMER1_MODE_PHASEFREQ_CORRECT_ICR                                 \
  TCCR1B |= (1 << WGM13);                                                     \
  TCCR1B &= ~(1 << WGM12);                                                    \
  TCCR1A &= ~((1 << WGM11) | (1 << WGM10))
// #define SET_TIMER1_MODE_PHASEFREQ_CORRECT_OCRA TCCR1B |= (1 << WGM13) | (0
// << WGM12);TCCR1A |= (0 << WGM11) | (1 << WGM10)
#define SET_TIMER1_MODE_PHASEFREQ_CORRECT_OCRA                                \
  TCCR1B |= (1 << WGM13);                                                     \
  TCCR1B &= ~(1 << WGM12);                                                    \
  TCCR1A &= ~(1 << WGM11);                                                    \
  TCCR1A |= (1 << WGM10)

// #define SET_TIMER1_MODE_PHASE_CORRECT_ICR TCCR1B |= (1 << WGM13) | (0 <<
// WGM12);TCCR1A |= (1 << WGM11) | (0 << WGM10)
#define SET_TIMER1_MODE_PHASE_CORRECT_ICR                                     \
  TCCR1B |= (1 << WGM13);                                                     \
  TCCR1B &= ~(1 << WGM12);                                                    \
  TCCR1A |= (1 << WGM11);                                                     \
  TCCR1A &= ~(1 << WGM10)

// #define SET_TIMER1_MODE_CTC_ICR TCCR1B |= (1 << WGM13) | (1 << WGM12);TCCR1A
// |= (0 << WGM11) | (0 << WGM10)
#define SET_TIMER1_MODE_CTC_ICR                                               \
  TCCR1B |= (1 << WGM13) | (1 << WGM12);                                      \
  TCCR1A &= ~((1 << WGM11) | (1 << WGM10))

// #define SET_TIMER1_MODE_FASTPWM_ICR TCCR1B |= (1 << WGM13) | (1 <<
// WGM12);TCCR1A |= (1 << WGM11) | (0 << WGM10)
#define SET_TIMER1_MODE_FASTPWM_ICR                                           \
  TCCR1B |= (1 << WGM13) | (1 << WGM12);                                      \
  TCCR1A |= (1 << WGM11);                                                     \
  TCCR1A &= ~(1 << WGM10)

// 0 0 1 0
#define SET_TIMER1_MODE_PHASE_CORRECT_TOP_9BIT                                \
  TCCR1B &= ~((1 << WGM13) | (1 << WGM12));                                   \
  TCCR1A |= (1 << WGM11);                                                     \
  TCCR1A &= ~(1 << WGM10)
//  0 0 1 1
#define SET_TIMER1_MODE_PHASE_CORRECT_TOP_10BIT                               \
  TCCR1B &= ~((1 << WGM13) | (1 << WGM12));                                   \
  TCCR1A |= (1 << WGM11) | (1 << WGM10)
// 0 1 1 0
#define SET_TIMER1_MODE_FASTPWM_TOP_9BIT                                      \
  TCCR1B &= ~(1 << WGM13);                                                    \
  TCCR1B |= (1 << WGM12);                                                     \
  TCCR1A |= (1 << WGM11);                                                     \
  TCCR1A &= ~(1 << WGM10)
// 0 1 1 1
#define SET_TIMER1_MODE_FASTPWM_TOP_10BIT                                     \
  TCCR1B &= ~(1 << WGM13);                                                    \
  TCCR1B |= (1 << WGM12);                                                     \
  TCCR1A |= (1 << WGM11) | (1 << WGM10)

// #define SET_TIMER1_FREQUENCY_ICRTOP(val) ICR1 = val
#define SET_TIMER1_ICR(val) cli();ICR1 = val;sei()

// #define SET_TIMER1_FREQUENCY_OCRATOP(val) OCR1A = val
// #define SET_TIMER1_DUTY_CHAN_A(val) OCR1A = val
#define SET_TIMER1_OCRA(val) cli();OCR1A = val;sei()

// #define SET_TIMER1_DUTY_CHAN_B(val) OCR1B = val
#define SET_TIMER1_OCRB(val) cli();OCR1B = val;sei()



#define ATOMIC_READ_16(reg) ({ \
    uint16_t _val; \
    uint8_t _sreg = SREG; \
    cli(); \
    _val = reg; \
    SREG = _sreg; \
    _val; \
})

#define GET_TIMER1_OCRA() ATOMIC_READ_16(OCR1A)
#define GET_TIMER1_OCRB() ATOMIC_READ_16(OCR1B)
#define GET_TIMER1_ICR()  ATOMIC_READ_16(ICR1)




// Timer1 Pins

// #define SET_TIMER1_PINB DDRB = DDRB | 0b00000100
// #define SET_TIMER1_PINA DDRB = DDRB | 0b00000010

#define SET_TIMER1_PINA DDRB |= (1 << PB1) // pin 9
#define SET_TIMER1_PINB DDRB |= (1 << PB2) // pin 10
#define SET_TIMER1_PINI                                                       \
  DDRB |= (1 << PB0) // pin 11 Input Capture pin interrupt pin ICP1

#define UNSET_TIMER1_PINA DDRB &= ~(1 << PB1) // pin 9
#define UNSET_TIMER1_PINB DDRB &= ~(1 << PB2) // pin 10
#define UNSET_TIMER1_PINI                                                     \
  DDRB &= ~(1 << PB0) // pin 11 Input Capture pin interrupt pin ICP1

#define SET_TIMER1_PINA_SET PORTB |= (1 << PB1)    // pin 9
#define SET_TIMER1_PINA_CLEAR PORTB &= ~(1 << PB1) // pin 9

#define SET_TIMER1_PINB_SET PORTB |= (1 << PB2)    // pin 10
#define SET_TIMER1_PINB_CLEAR PORTB &= ~(1 << PB2) // pin 10

/*
//*COM1A1/COM1B1 COM1A0/COM1B0
#define SET_TIMER1_PINOUT(pin) TCCR1A |= 1 << COM1##pin##1
#define UNSET_TIMER1_PINOUT(pin) TCCR1A &= ~(1 << COM1##pin##1)

#define SET_TIMER1_PINB_NOTINVERTING(notInverting) \ TCCR1A |= notInverting <<
COM1B0
#define SET_TIMER1_PINA_NOTINVERTING(notInverting) \ TCCR1A |= notInverting <<
COM1A0
*/

#define SET_TIMER1_PINA_DISABLE TCCR1A &= ~(1 << COM1A1) | ~(1 << COM1A0)
// COM0A1=0, COM0A0=1 per TIMER1 valido solo in NORMAL e CTC
#define SET_TIMER1_PINA_TOGGLE_ONCOMPARE                                      \
  TCCR1A &= ~(1 << COM1A1);                                                   \
  TCCR1A |= (1 << COM1A0)
// COM0A1=1, COM0A0=0
#define SET_TIMER1_PINA_CLEAR_ONCOMPARE                                       \
  TCCR1A |= (1 << COM1A1);                                                    \
  TCCR1A &= ~(1 << COM1A0)
// COM0A1=1, COM0A0=1
#define SET_TIMER1_PINA_SET_ONCOMPARE TCCR1A |= (1 << COM1A1) | (1 << COM1A0)

#define SET_TIMER1_PINB_DISABLE TCCR1A &= ~(1 << COM1B1) | ~(1 << COM1B0)
// COM0A1=0, COM0A0=1 per TIMER1 valido solo in NORMAL e CTC
#define SET_TIMER1_PINB_TOGGLE_ONCOMPARE                                      \
  TCCR1A &= ~(1 << COM1B1);                                                   \
  TCCR1A |= (1 << COM1B0)
// COM0A1=1, COM0A0=0
#define SET_TIMER1_PINB_CLEAR_ONCOMPARE                                       \
  TCCR1A |= (1 << COM1B1);                                                    \
  TCCR1A &= ~(1 << COM1B0)
// COM0A1=1, COM0A0=1
#define SET_TIMER1_PINB_SET_ONCOMPARE TCCR1A |= (1 << COM1B1) | (1 << COM1B0)

// Timer1 interrupt

#define SET_TIMER1_INTERRUPT_INPUTCAPTURE TIMSK1 |= (1 << ICIE1)
#define SET_TIMER1_INTERRUPT_OUTPUTCOMPARE_B TIMSK1 |= (1 << OCIE1B)
#define SET_TIMER1_INTERRUPT_OUTPUTCOMPARE_A TIMSK1 |= (1 << OCIE1A)
#define SET_TIMER1_INTERRUPT_OVERFLOW TIMSK1 |= (1 << TOIE1)

#define UNSET_TIMER1_INTERRUPT_INPUTCAPTURE TIMSK1 &= ~(1 << ICIE1)
#define UNSET_TIMER1_INTERRUPT_OUTPUTCOMPARE_B TIMSK1 &= ~(1 << OCIE1B)
#define UNSET_TIMER1_INTERRUPT_OUTPUTCOMPARE_A TIMSK1 &= ~(1 << OCIE1A)
#define UNSET_TIMER1_INTERRUPT_OVERFLOW TIMSK1 &= ~(1 << TOIE1)



#define DEFAULT_T1_INIT_FREQUENCY 1 // 244 //Hz-> TOP:65534
#define DEFAULT_T1_INIT_DUTY 1
#define DEFAULT_T1_INIT_PRESCALER 1

/*
#ifndef UINT16_MAX
#define UINT16_MAX 65534
#endif
*//*
extern uint16_t timer1_minHzPrescaler1;
extern uint16_t timer1_minHzPrescaler8;
extern uint16_t timer1_minHzPrescaler64;
extern uint16_t timer1_minHzPrescaler256;
*/

extern void (*timer1_incmp_handler)();
extern void (*timer1_ovf_handler)();
extern void (*timer1_compa_handler)();
extern void (*timer1_compb_handler)();


// Variables
//extern uint16_t timer1_prescaler;

extern timer_mode timer1_mode;
extern uint16_t timer1_prescaler;
extern volatile uint8_t timer1_bottom;
extern volatile uint16_t timer1_top;
extern uint8_t timer1_duty;
extern uint8_t timer1_running;
extern uint16_t timer1_allowedPrescalers[];
extern uint8_t timer1_allowedPrescalersLenght;




extern void (*myfunc) (void);

// functions
void timer1_register_INCMP_callback (void (*func) (void));
void timer1_register_COMPA_callback(void (*func)(void));
void timer1_register_COMPB_callback(void (*func)(void));
void timer1_register_OVF_callback(void (*func)(void));



// Functions
int8_t timer1_init ();
void timer1_init(timer_mode _mode, uint16_t _prescaler);

void timer1_mode_normal_init(void);
void timer1_mode_phasecorrect_ocra_init(void);
void timer1_mode_ctc_ocra_init(void);
void timer1_mode_phasecorrect_top_init(void);
void timer1_mode_fastpwm_top_init(void);
     
void timer1_mode_fastpwm_ocra_init(void);
void timer1_mode_fastpwm_icr_init(void);

void timer1_setPin(char _pin, timer_pin_mode _mode);
void timer1_clearPin(char _pin);


void timer1_start (uint32_t Hz);
void timer1_start(uint16_t _prescaler);
void timer1_start(void);
uint8_t timer1_isRunning(); 
void timer1_stop ();

void timer1_resetCounter(void);
void timer1_setPrescaler(uint16_t _prescaler);
void timer1_increasePrescaler(void);
void timer1_decreasePrescaler(void);

uint16_t timer1_getNextPrescaler(void);
uint16_t timer1_getPrevPrescaler(void);
//uint16_t timer1_getPrescaler(void);
//uint32_t timer1_getFrequency(void);

//void timer1_getPrescalerMinHz (void);
//uint16_t timer1_getPrescalerRequired (uint32_t Hz);
//int8_t timer1_setPrescaler (uint16_t _prescaler);
int8_t timer1_setFrequency (uint32_t Hz);
int8_t timer1_setTop (uint16_t top);
void timer1_setBottom(uint8_t _bottom);
int8_t timer1_setDuty (uint8_t duty);
uint16_t timer1_getPrescaler (void);
uint16_t timer1_getFrequency (void);
uint16_t timer1_getTop (void);
uint16_t timer1_getDuty (void);

void timer1_timer1_ovf_handler (void);

#endif // header
