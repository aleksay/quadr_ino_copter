#ifndef mosfetSeqController_h
#define mosfetSeqController_h

#include "Arduino.h"
#include "atmegax8.h"
													 // Decimal representation	
const uint8_t states[NUM_STATES] = {	0b01000100,  //  68
										0b10000100,  // 132
										0b10001000,  // 136
										0b00101000,  //  40
										0b00110000,  //  48
										0b01010000 };//  80

class mosfetSequencecontroller {

public:

	mosfetSequencecontroller();

	int init();
	int getDirection();
	int setDirection(int clockwise);
	int commutePole();
	int setAutomaRate(int val);
	int getAutomaRate();
	int getAutomaState();
	int getState();
	int getAutomaState(int st);
private:

};

#endif

