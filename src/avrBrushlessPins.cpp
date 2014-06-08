#include "avrBrushlessPins.h"




static volatile uint8_t driveState;
static volatile uint8_t driveDirection = 1;
static unsigned char driveTable[NUM_DRIVE_STATES];


static void pinsMakeTables(void)

{

  driveTable[0] = DRIVE_PATTERN_STEP1;
  driveTable[1] = DRIVE_PATTERN_STEP2;
  driveTable[2] = DRIVE_PATTERN_STEP3;
  driveTable[3] = DRIVE_PATTERN_STEP4;
  driveTable[4] = DRIVE_PATTERN_STEP5;
  driveTable[5] = DRIVE_PATTERN_STEP6;


  //  ADMUXTable[0] = ADMUX_W;
  //  ADMUXTable[1] = ADMUX_V;
  //  ADMUXTable[2] = ADMUX_U;
  //  ADMUXTable[3] = ADMUX_W;
  //  ADMUXTable[4] = ADMUX_V;
  //  ADMUXTable[5] = ADMUX_U;
}

void pins_init(void) {

  pinsMakeTables();

  // Init DRIVE_DDR for motor driving.
  DRIVE_DDR = (1 << UL_OFFSET) | (1 << UH_OFFSET) | (1 << VL_OFFSET) | (1 << VH_OFFSET) | (1 << WL_OFFSET) | (1 << WH_OFFSET);
  pins_setDriveState(0);
}


uint8_t pins_getDriveState(void) {
  return driveState;
}

int8_t pins_setDriveState(uint8_t _state) {

  if (_state < 0 || _state > NUM_DRIVE_STATES) {
    return -1;
  }

  driveState = _state;
  DRIVE_PORT = driveTable[_state];

  return 0;
}



void pins_commuteDriveTable(void) {

  if (driveDirection) {
    driveState = ++driveState % NUM_DRIVE_STATES;
    DRIVE_PORT = driveTable[driveState];
  }
  else {
    driveState = (--driveState + NUM_DRIVE_STATES) % NUM_DRIVE_STATES;
    DRIVE_PORT = driveTable[driveState];
  }
}


void pins_setDriveDirection(uint8_t clockwise) {
  driveDirection = clockwise;
}
uint8_t pins_getDriveDirection() {
  return driveDirection;
}

void pins_setDriveOpenInverter() {
  DRIVE_PATTERN_OPEN_INVERTER;
}







