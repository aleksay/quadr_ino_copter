#ifndef BRUSHLESS_PINS_H
#define BRUSHLESS_PINS_H

#include <avrSequenceEncoder.h>

#define DRIVE_PATTERN_STEP1 ((1 << UL_OFFSET) | (1 << VH_OFFSET))
#define DRIVE_PATTERN_STEP2 ((1 << UL_OFFSET) | (1 << WH_OFFSET))
#define DRIVE_PATTERN_STEP3 ((1 << VL_OFFSET) | (1 << WH_OFFSET))
#define DRIVE_PATTERN_STEP4 ((1 << VL_OFFSET) | (1 << UH_OFFSET))
#define DRIVE_PATTERN_STEP5 ((1 << WL_OFFSET) | (1 << UH_OFFSET))
#define DRIVE_PATTERN_STEP6 ((1 << WL_OFFSET) | (1 << VH_OFFSET))

#define NUM_POLES 12
#define NUM_DRIVE_STATES 6
#define DEFAULT_INITIAL_STATE 0

void pins_init(void);
uint8_t pins_getDriveState(void);
void pins_setDriveState(uint8_t _state);
void pins_commuteDriveTable(void);
void pins_setDriveDirection(uint8_t clockwise);
uint8_t pins_getDriveDirection();
void pins_setDriveOpenInverter();

#endif
