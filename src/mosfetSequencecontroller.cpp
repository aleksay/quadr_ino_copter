#include "mosfetSequencecontroller.h"

#include "atmegax8.h"

#define DEFAULT_INITIAL_RATE 100
#define DEFAULT_INITIAL_STATE 0

volatile int state;
int rate;
volatile int direction;

mosfetSequencecontroller::mosfetSequencecontroller() {
	state = DEFAULT_INITIAL_STATE;
	rate = DEFAULT_INITIAL_RATE;
	direction = 0;
}

int mosfetSequencecontroller::init() {
	SET_AUTOMA_PORTD;
	AUTOMA_ITERATE(state);
}

int mosfetSequencecontroller::iterate() {

	if (direction) {
		AUTOMA_ITERATE(++state % NUM_STATES);
	} else {
		AUTOMA_ITERATE((NUM_STATES-1 + state--) %NUM_STATES);
	}
	return 0;
}
int mosfetSequencecontroller::setAutomaRate(int val) {
	rate = val;
}
int mosfetSequencecontroller::getAutomaRate() {
	return rate;
}

int mosfetSequencecontroller::setDirection(int clockwise) {
	direction = clockwise;
}
int mosfetSequencecontroller::getDirection() {
	return direction;
}

