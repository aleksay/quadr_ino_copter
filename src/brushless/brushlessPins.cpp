#include <brushlessPins.h>

void
pins_init ()
{

  unsigned char driveTable[]
      = { DRIVE_PATTERN_STEP1, DRIVE_PATTERN_STEP2, DRIVE_PATTERN_STEP3,
          DRIVE_PATTERN_STEP4, DRIVE_PATTERN_STEP5, DRIVE_PATTERN_STEP6 };

  sequence_init ();
  setSequenceTable (driveTable);
  sequence_setState (DEFAULT_INITIAL_STATE);
}

uint8_t
pins_getDriveState ()
{
  return sequence_getState ();
}

void
pins_setDriveState (uint8_t _state)
{
  sequence_setState (_state);
}

void
pins_commuteDriveTable (void)
{
  sequence_nextState ();
}
void
pins_setDriveDirection (uint8_t clockwise)
{
  sequence_setDirection (clockwise);
}
uint8_t
pins_getDriveDirection ()
{
  return sequence_getDirection ();
}
void
pins_setDriveOpenInverter ()
{
  sequence_erase ();
}
