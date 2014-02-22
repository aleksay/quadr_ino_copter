#ifndef mosfetSeqController_h
#define mosfetSeqController_h

#include "Arduino.h"
#include "atmegax8.h"

const uint8_t states[NUM_STATES] = {	0b00010001,  //  68
					0b00100001,  // 132
					0b00100010,  // 136
					0b00001010,  //  40
					0b00001100,  //  48
					0b00010100 };//  80

class mosfetSequencecontroller {

public:

	mosfetSequencecontroller();

	int init();
	int getDirection();
	int setDirection(int clockwise);
	int commutePole();
	int setAutomaRate(int val);
	int getAutomaRate();
	int getbit();
	int getAutomaState();
	int getState();
	int getAutomaState(int st);
private:

};

#endif

