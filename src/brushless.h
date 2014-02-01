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
	int start;
	int end;
	float decrement;
	int currentValue;
	float resto;
}*startupData;

class brushless {

public:
	brushless();
	int startup();
	void iterate();
	int start();

	int setCommand(Command command);  
	String getResponse();

private:

	Command latestCommand;
	int startupping = 0;
  int commandRead = 0;
  String latestMessage;
	


	String parseCommand(Command command);
  void startupcalc(startupData valueData, int slow);
};

#endif
