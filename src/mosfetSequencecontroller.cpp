#include "mosfetSequencecontroller.h"

#include "atmegax8.h"


#define DEFAULT_INITIAL_RATE 100
#define DEFAULT_INITIAL_STATE 0

volatile int state;
volatile int rate;
volatile int direction;



mosfetSequencecontroller::mosfetSequencecontroller() {
	state     = DEFAULT_INITIAL_STATE;
	rate      = DEFAULT_INITIAL_RATE;
	direction = 1;
 
}

int mosfetSequencecontroller::getAutomaState(){
	return states[state];
}

int mosfetSequencecontroller::getState(){
	return state;
}

int mosfetSequencecontroller::getAutomaState(int st){  
	return states[st];
}

int mosfetSequencecontroller::init() {

  AUTOMA_PIN_INIT;
  AUTOMA_ITERATE(state);
}

int mosfetSequencecontroller::commutePole() {

	if (direction) {
		state = ++state % NUM_STATES;
		AUTOMA_ITERATE(state);
	} else {
    state = (--state +NUM_STATES)%NUM_STATES;
    //state = abs(NUM_STATES - 1 + --state) % NUM_STATES;
		AUTOMA_ITERATE(state);
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

//volatile int cpmCounter = 0;
ISR(TIMER1_COMPA_vect) {
	state = ++state % NUM_STATES;
	AUTOMA_ITERATE(state);
}










