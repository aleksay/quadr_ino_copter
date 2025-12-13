#ifndef AVR6STATESEQENCODER_H
#define AVR6STATESEQENCODER_H

#include <avr/io.h>
// #include <stdint.h>

#define SEQUENCE_SIZE 6

//! Clockwise rotation flag. Used only in macros.
// #define CW    0

//! Counterclockwise rotation flag. Used only in macros.
// #define CCW   1

// SEQUENCE TABLE PINOUTS
#define UH_OFFSET PORTB0 // ardu 8
#define UL_OFFSET PORTB3 // ardu 11
#define VH_OFFSET PORTB1 // ardu 9
#define VL_OFFSET PORTB4 // ardu 12
#define WH_OFFSET PORTB2 // ardu 10
#define WL_OFFSET PORTB5 // ardu 13

//! PORT register for drive pattern output.
#define SEQUENCE_PORT PORTB

//! Data direction register for drive pattern output.
#define SEQUENCE_DDR DDRB

#define SEQUENCE_ERASE                                                        \
  PORTB |= (0 << PORTB0) | (0 << PORTB1) | (0 << PORTB2) | (0 << PORTB3)      \
           | (0 << PORTB4) | (0 << PORTB5)

/*
#define SEQUENCE_PATTERN_STEP1      ((1 << UL_OFFSET) | (1 << VH_OFFSET))
#define SEQUENCE_PATTERN_STEP2      ((1 << UL_OFFSET) | (1 << WH_OFFSET))
#define SEQUENCE_PATTERN_STEP3      ((1 << VL_OFFSET) | (1 << WH_OFFSET))
#define SEQUENCE_PATTERN_STEP4      ((1 << VL_OFFSET) | (1 << UH_OFFSET))
#define SEQUENCE_PATTERN_STEP5      ((1 << WL_OFFSET) | (1 << UH_OFFSET))
#define SEQUENCE_PATTERN_STEP6      ((1 << WL_OFFSET) | (1 << VH_OFFSET))
*/

//! Macro that cuts all power to the motor.
// #define DISABLE_DRIVING               (SEQUENCE_PORT = 0x00)

void setSequenceTable (unsigned char *_sequenceTable);

void sequence_init (void);
uint8_t sequence_getState (void);
void sequence_setState (uint8_t _state);
void sequence_setDirection (uint8_t clockwise);
uint8_t sequence_getDirection ();
void sequence_erase ();

void sequence_nextState (void);

static volatile uint8_t state;
static volatile uint8_t direction;
static unsigned char *sequenceTable; //[SEQUENCE_SIZE];

#endif
