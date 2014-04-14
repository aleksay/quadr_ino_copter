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


#define RAMP_FIN_FREQUENCY_T1 5000
#define RAMP_FIN_DUTY_T0 90



typedef struct _startup_data {
  unsigned int start;
  unsigned int end;
  float gain;
  unsigned int currentValue;
  float resto;
}
*startupData;

class brushless {

public:
  brushless();
  int startup();
  void iterate();
  void start();
  int setCommand(Command command);  
  String getResponse();
  void setTime();
  int Hz2top();
  float angSpeed();
private:

  Command latestCommand;
  int startupping;
  int commandRead;
  String latestMessage;
  int msTime;

  String parseCommand(Command command);
  //void startupcalc(startupData valueData, int slow);
  int getStartupValueHz(int gain, int ssGain);
};

#endif

