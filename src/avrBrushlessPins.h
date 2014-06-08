#ifndef avrBrushlessPins_h
#define avrBrushlessPins_h

#include <avr/io.h>
#include <stdint.h>

#define NUM_DRIVE_STATES 6

//! Clockwise rotation flag. Used only in macros.
#define CW    0

//! Counterclockwise rotation flag. Used only in macros.
#define CCW   1

// DRIVE TABLE PINOUTS
#define UH_OFFSET PORTB0 // ardu 8
#define UL_OFFSET PORTB3 // ardu 11
#define VH_OFFSET PORTB1 // ardu 9
#define VL_OFFSET PORTB4 // ardu 12
#define WH_OFFSET PORTB2 // ardu 10
#define WL_OFFSET PORTB5 // ardu 13


//! PORT register for drive pattern output.
#define DRIVE_PORT  PORTB

//! Data direction register for drive pattern output.
#define DRIVE_DDR   DDRB


#define DRIVE_PATTERN_OPEN_INVERTER     PORTB = 0b00000000 // TODO go in OR to not reset last 2 bits which are not used
#define DRIVE_PATTERN_STEP1      ((1 << UL_OFFSET) | (1 << VH_OFFSET))
#define DRIVE_PATTERN_STEP2      ((1 << UL_OFFSET) | (1 << WH_OFFSET))
#define DRIVE_PATTERN_STEP3      ((1 << VL_OFFSET) | (1 << WH_OFFSET))
#define DRIVE_PATTERN_STEP4      ((1 << VL_OFFSET) | (1 << UH_OFFSET))
#define DRIVE_PATTERN_STEP5      ((1 << WL_OFFSET) | (1 << UH_OFFSET))
#define DRIVE_PATTERN_STEP6      ((1 << WL_OFFSET) | (1 << VH_OFFSET))

//! Macro that cuts all power to the motor.
#define DISABLE_DRIVING               (DRIVE_PORT = 0x00)

void pins_init(void);
uint8_t pins_getDriveState(void);
int8_t pins_setDriveState(uint8_t _state);
void pins_commuteDriveTable(void);
void pins_setDriveDirection(uint8_t clockwise);
uint8_t pins_getDriveDirection();
void pins_setDriveOpenInverter();


#endif


