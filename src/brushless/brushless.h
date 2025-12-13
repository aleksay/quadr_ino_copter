#ifndef BRUSHLESS_H
#define BRUSHLESS_H

// #include <Arduino.h>

// #include "comMessage.h"
// #include "comLogger.h"
// #include <comunicator.h>

// #include "avrBrushlessPins.h"
// #include <avrSequenceEncoder.h>
#include <brushlessPins.h>

// #include "avrUtils.h"
#include "brushlessInterrupt.h"
#include "brushlessPWM.h"
#include "time.h"

// TODO change to startupState_MotorOff with __UNDERSCORE__
#define startupState_MotorOff 0
#define startupState_MotorInit 1
#define startupState_PWMStarted 2
#define startupState_RotorAligned 3
#define startupState_SetupAutomaRampA 4
#define startupState_AutomaRampA 5
#define startupState_SetupAutomaRampB 6
#define startupState_AutomaRampB 7
#define startupState_StartupFinished 8

typedef struct _ramp
{
  int offset;
  int gain;
  int currentValue;
  int end;
} ramp;

// class brushless {

// public:

//   brushless();
//   ~brushless();

// methods
int init ();
int iterate ();
int setStartupFreqEnd (int val);
int setStartupFreqGain (int val);
// int setCommand(Command command);
void manualMode (uint8_t val);
void angSpeed (uint8_t val);

// private:
void ramp_init ();

// methods
// int parseCommand(Command command);

int getStartupOpenLoopValue (ramp ramp);
void setStartupState (uint8_t state);
int startupCallback ();

//};

#endif
