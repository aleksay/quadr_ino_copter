#include <avrSequenceEncoder.h>

void setSequenceTable(unsigned char *_sequenceTable) {
  sequenceTable = _sequenceTable;
}

/*
static void pinsMakeTables(void)

{

  sequenceTable[0] = SEQUENCE_PATTERN_STEP1;
  sequenceTable[1] = SEQUENCE_PATTERN_STEP2;
  sequenceTable[2] = SEQUENCE_PATTERN_STEP3;
  sequenceTable[3] = SEQUENCE_PATTERN_STEP4;
  sequenceTable[4] = SEQUENCE_PATTERN_STEP5;
  sequenceTable[5] = SEQUENCE_PATTERN_STEP6;


  //  ADMUXTable[0] = ADMUX_W;
  //  ADMUXTable[1] = ADMUX_V;
  //  ADMUXTable[2] = ADMUX_U;
  //  ADMUXTable[3] = ADMUX_W;
  //  ADMUXTable[4] = ADMUX_V;
  //  ADMUXTable[5] = ADMUX_U;
}
*/
void sequence_init(void) {
  direction = 1;
  // pinsMakeTables();

  // Init DRIVE_DDR for motor driving.
  SEQUENCE_DDR = (1 << UL_OFFSET) | (1 << UH_OFFSET) | (1 << VL_OFFSET) |
                 (1 << VH_OFFSET) | (1 << WL_OFFSET) | (1 << WH_OFFSET);

  sequence_setState(0);
}

uint8_t sequence_getState() { return state; }

void sequence_setState(uint8_t _state) {

  state = _state;
  SEQUENCE_PORT = sequenceTable[_state];
}

void sequence_nextState(void) {

  if (direction) {
    state = ++state % SEQUENCE_SIZE;
    SEQUENCE_PORT = sequenceTable[state];
  } else {
    state = (--state + SEQUENCE_SIZE) % SEQUENCE_SIZE;
    SEQUENCE_PORT = sequenceTable[state];
  }
}

void sequence_setDirection(uint8_t clockwise) { direction = clockwise; }
uint8_t sequence_getDirection() { return direction; }

void sequence_erase() { SEQUENCE_ERASE; }
