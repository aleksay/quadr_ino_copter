/*
 Header file for brshless control module

 */

#ifndef BRUSHLESS_h
#define BRUSHLESS_h

#include <Arduino.h>
#include "comMessage.h"
#include "comLogger.h"
#include "avrBrushlessPins.h"
#include "avrUtils.h"
#include "avrTime.h"
#include "brushlessInterrupt.h"
#include "brushlessPWM.h"

//TODO change to startupState_MotorOff with __UNDERSCORE__
#define startupState_MotorOff 0
#define startupState_MotorInit 1
#define startupState_PWMStarted 2
#define startupState_RotorAligned 3
#define startupState_SetupAutomaRampA 4
#define startupState_AutomaRampA 5
#define startupState_SetupAutomaRampB 6
#define startupState_AutomaRampB 7
#define startupState_StartupFinished 8


typedef struct ramp {
  int offset;
  int gain;
  int currentValue;
  int end;
}
ramp;



class brushless {

  public:

    brushless();
    ~brushless();

    // methods
	int init();
    int iterate();
    int setStartupFreqEnd (int val);
    int setStartupFreqGain (int val);
    int setCommand(Command command);

  private:


    // methods
    int parseCommand(Command command);
    int angSpeed();
    int getStartupOpenLoopValue(ramp ramp);
    int setStartupState(int state);
    int startupCallback();
    int manualMode();

    // variables
    int starting;
    int startupState;
    uint32_t startTime;
    uint32_t TotStartupTime;
    int commandRead;
    Command latestCommand;
    ramp rampPWMDuty, rampAutomaFrequencyA, rampAutomaFrequencyB;

};

#endif


