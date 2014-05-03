/*
 Header file for brshless control module
 
 */

#ifndef BRUSHLESS_h
#define BRUSHLESS_h

#include <Arduino.h>
#include <WString.h>
#include "communicator.h"
#include "timer.h"
#include "mosfetSequencecontroller.h"


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
}ramp;



class brushless {

public:

  brushless();
  ~brushless();

  // methods
  int iterate();  
  int incrementTime();
  int setStartupFreqEnd (int val);
  int setStartupFreqGain (int val);
  int start();
  int setCommand(Command command);
  String getResponse();

private:

  // methods
  String parseCommand(Command command);
  String angSpeed();
  int getStartupOpenLoopValue(ramp ramp);
  int setStartupState(int state);
  int startupCallback();
  
  // variables
  int starting;
  int startupState;
  unsigned int msTime;
  int commandRead;
  String latestMessage;
  Command latestCommand;
  ramp rampPWMDuty, rampAutomaFrequencyA, rampAutomaFrequencyB;

};

#endif

