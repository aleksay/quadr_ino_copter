#ifndef ATMEGAX8
#define ATMEGAX8 

#include <Arduino.h>

/*
=====================================================
 
 		           Global configuration Section
 
 =====================================================
 */

//#define F_CPU 8000000UL  //Uncomment for arduino Fio
#define F_CPU 16000000UL  //Uncomment for arduino duemilanove

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

#define SET_TIMER0_STOP           TCCR0B |= (0 << CS02) | (0 << CS01) | (0 << CS00)
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

/*
=====================================================
 
 		           Timer 1 Section
 
 =====================================================
 */
#define TIMER1_RESET TCCR1B=0;TCCR1A=0;TCNT1=0

/*
 CS12 CS11 CS10 Description
 0 0 0 No clock source (Timer/Counter stopped).
 0 0 1 clkI/O/0 (No prescaling)
 0 1 0 clkI/O/8 (From prescaler)
 0 1 1 clkI/O/64 (From prescaler)
 1 0 0 clkI/O/256 (From prescaler)
 1 0 1 clkI/O/1024 (From prescaler)
 */
#define SET_TIMER1_STOP           TCCR1B |= (0 << CS12) | (0 << CS11) | (0 << CS10)
#define SET_TIMER1_PRESCALER_1    TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10)
#define SET_TIMER1_PRESCALER_8    TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10)
#define SET_TIMER1_PRESCALER_64   TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10)
#define SET_TIMER1_PRESCALER_256  TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10)
#define SET_TIMER1_PRESCALER_1024 TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10)



/*
 WGM13 | WGM12 | WGM11 | WGM10 | Timer/Counter Mode of Operation | TOP | Update of OCR1x at | TOV1 Flag Set on
 4  0 1 0 0 CTC OCR1A Immediate MAX
 8  1 0 0 0 PWM, Phase and Frequency Correct ICR1 BOTTOM BOTTOM
 9  1 0 0 1 PWM, Phase and Frequency Correct OCR1A BOTTOM BOTTOM
 10 1 0 1 0 PWM, Phase Correct ICR1 TOP BOTTOM
 11 1 0 1 1 PWM, Phase Correct OCR1A TOP BOTTOM
 12 1 1 0 0 CTC ICR1 Immediate MAX
 14 1 1 1 0 Fast PWM ICR1 BOTTOM TOP
 15 1 1 1 1 Fast PWM OCR1A BOTTOM TOP
 */
#define SET_TIMER1_MODE_CTC_OCR1A               TCCR1B |= (0 << WGM13) | (1 << WGM12);TCCR1A |= (0 << WGM11) | (0 << WGM10)
#define SET_TIMER1_MODE_PHASEFREQ_CORRECT_ICR1  TCCR1B |= (1 << WGM13) | (0 << WGM12);TCCR1A |= (0 << WGM11) | (0 << WGM10)
#define SET_TIMER1_MODE_PHASEFREQ_CORRECT_OCR1A TCCR1B |= (1 << WGM13) | (0 << WGM12);TCCR1A |= (0 << WGM11) | (1 << WGM10)
#define SET_TIMER1_MODE_PHASE_CORRECT_ICR1      TCCR1B |= (1 << WGM13) | (0 << WGM12);TCCR1A |= (1 << WGM11) | (0 << WGM10)
#define SET_TIMER1_MODE_PHASE_CORRECT_OCR1A     TCCR1B |= (1 << WGM13) | (0 << WGM12);TCCR1A |= (1 << WGM11) | (1 << WGM10)
#define SET_TIMER1_MODE_CTC_ICR1                TCCR1B |= (1 << WGM13) | (1 << WGM12);TCCR1A |= (0 << WGM11) | (0 << WGM10)
#define SET_TIMER1_MODE_FASTPWM_ICR1            TCCR1B |= (1 << WGM13) | (1 << WGM12);TCCR1A |= (1 << WGM11) | (0 << WGM10)
#define SET_TIMER1_MODE_FASTPWM_OCR1A           TCCR1B |= (1 << WGM13) | (1 << WGM12);TCCR1A |= (1 << WGM11) | (1 << WGM10)

/*
 Bit 5 – ICIE1:  Timer/Counter1, Input Capture Interrupt Enable
 Bit 2 – OCIE1B: Timer/Counter1, Output Compare B Match Interrupt Enable
 Bit 1 – OCIE1A: Timer/Counter1, Output Compare A Match Interrupt Enable
 Bit 0 – TOIE1:  Timer/Counter1, Overflow Interrupt Enable
 */
#define SET_TIMER1_INTERRUPT_INPUTCAPTURE    TIMSK1 |= (1 << ICIE1 )
#define SET_TIMER1_INTERRUPT_OUTPUTCOMPARE_B TIMSK1 |= (1 << OCIE1B)
#define SET_TIMER1_INTERRUPT_OUTPUTCOMPARE_A TIMSK1 |= (1 << OCIE1A)
#define SET_TIMER1_INTERRUPT_OVERFLOW        TIMSK1 |= (1 << TOIE1 )

#define SET_TIMER1_PINB DDRB = DDRB | 0b00000100
#define SET_TIMER1_PINA DDRB = DDRB | 0b00000010
/*COM1A1/COM1B1 COM1A0/COM1B0*/
#define SET_TIMER1_PINOUT(pin)   TCCR1A |= 1 << COM1##pin##1 
#define UNSET_TIMER1_PINOUT(pin) TCCR1A &= ~(1 << COM1##pin##1) 

#define SET_TIMER1_PINB_NOTINVERTING(notInverting) TCCR1A |= notInverting << COM1B0
#define SET_TIMER1_PINA_NOTINVERTING(notInverting) TCCR1A |= notInverting << COM1A0

#define SET_TIMER1_FREQUENCY_ICR1TOP(val)  ICR1  = val
#define SET_TIMER1_FREQUENCY_OCR1ATOP(val) OCR1A = val
#define SET_TIMER1_DUTY_CHAN_B(val)        OCR1B = val
#define SET_TIMER1_DUTY_CHAN_A(val)        OCR1A = val


/*
=====================================================
 
 		Mosfet Sequence Controller Section
 
 =====================================================
 */
//#define	automa_register_D
#define automa_register_B


#ifdef automa_register_D
#define AUTOMA_PIN_INIT DDRD |= 0b00111111 
#define AUTOMA_ITERATE(state) PORTD=states[state]
#endif

#ifdef automa_register_B
#define AUTOMA_PIN_INIT DDRB |= 0b00111111 
#define AUTOMA_ITERATE(state) PORTB=states[state]
#endif



#endif

