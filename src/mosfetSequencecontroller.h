#ifndef mosfetSeqController_h
#define mosfetSeqController_h

#include "Arduino.h"
#include "atmegax8.h"

PROGMEM const byte states[NUM_STATES] = { 0b01000100, 
  																				0b10000100, 
																					0b10001000,
																					0b00101000, 
																					0b00110000, 
																					0b01010000 };

class mosfetSequencecontroller {

public:

	mosfetSequencecontroller();

	int init();
	int getDirection();
	int setDirection(int clockwise);
	int iterate();
	int setAutomaRate(int val);
	int getAutomaRate();

private:

};

#endif

