/*
 Header file for brshless control module

 */

#ifndef BRUSHLESS_h
#define BRUSHLESS_h

#include <Arduino.h>
#include <WString.h>
#include "communicator.h"

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

	//timer.cpp
	/*   int setFrequency(int val);
	 int setDuty(int val);
	 int getFrequency();
	 int getDuty();

	 //mosfetSequence 
	 int setRefreshRate(int val);
	 int getRefreshRate();
	 int eventHandler();
	 */
	int startup();
	void startupcalc(startupData valueData, int slow);
	void iterate();
	int start();
	int stop();

	int setCommand(Command command);  
	String getResponse();

private:
	volatile unsigned int cpmCounter;
	volatile int stato;
	volatile unsigned int frequency; //holds register value
	volatile unsigned int refreshRate;
	volatile unsigned int duty; //holds register value
	Command latestCommand;
	int startupping = 0;
  int commandRead = 0;
  String latestMessage;


	String parseCommand(Command command);
	//int timer1_init();
};

#endif
