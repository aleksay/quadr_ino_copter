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
#define startupStateMotorOff 0
#define startupStatePWMStarted 1
#define startupStateRotorAligned 2
#define startupStatePWMDutyIncreasing 3
#define startupStateAutomaStarted 4
#define startupStateFrequencyAutomaAndPWMDutyIncreasing 5
#define startupStateFrequencyAutomaIncreasing 6
#define startupStateStartupFinished 7


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
  int msTime;
  int commandRead;
  String latestMessage;
  Command latestCommand;
  ramp rampPWMDuty, rampAutomaFrequencyA, rampAutomaFrequencyB;

};

#endif

