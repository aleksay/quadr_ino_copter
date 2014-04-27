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






typedef struct _startup_data {
  unsigned int start;
  unsigned int end;
  int gain;
  //unsigned int currentValue;
}

*startupData;

class brushless {

public:
  brushless();
  void startup();
  void motor_init();
  void iterate();
  void start();
  int setCommand(Command command);  
  String getResponse();
  void incrementTime();
  int Hz2top();
  String angSpeed();
  void startupping();
  int setStartupfreqEnd (int val);
  int setStartupfreqGain (int val);
private:

  Command latestCommand;
  
  int commandRead;
  String latestMessage;
  unsigned int msTime;
  

  String parseCommand(Command command);
  int getStartupValueHz(int gain, int ssGain);
};

#endif

