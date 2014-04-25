#include "mosfetSequencecontroller.h"

#include "atmegax8.h"


volatile int state;
volatile int isRunning;
volatile int direction;



mosfetSequencecontroller::mosfetSequencecontroller() {
  state      = DEFAULT_INITIAL_STATE;
  isRunning  = 0;
  direction  = 1;

}

//int mosfetSequencecontroller::getAutomaState(){
//  return states[state];
//}

int mosfetSequencecontroller::getState(){
  return state;
}


int mosfetSequencecontroller::start(){
	isRunning = 1;
}

int mosfetSequencecontroller::stop(){
	isRunning = 0;
}



int mosfetSequencecontroller::setState(int _state){

	if(_state < 0 || _state >= NUM_STATES){
		return -1;
	}
	
	state =_state;
	AUTOMA_ITERATE(state);
	
	return 0;
}


int mosfetSequencecontroller::init() {

  AUTOMA_PIN_INIT;
  setState(0);
}

int mosfetSequencecontroller::commutePole() {

	if (isRunning == 0) { 
		return 0; 
	}

  if (direction) {
    state = ++state % NUM_STATES;
    AUTOMA_ITERATE(state);
  } 
  else {
    state = (--state +NUM_STATES)%NUM_STATES;
    //state = abs(NUM_STATES - 1 + --state) % NUM_STATES;
    AUTOMA_ITERATE(state);
  }
  return 0;
}
//int mosfetSequencecontroller::setAutomaRate(int val) {
//  rate = val;
//}
//int mosfetSequencecontroller::getAutomaRate() {
//  return rate;
//}

int mosfetSequencecontroller::setDirection(int clockwise) {
  direction = clockwise;
}
int mosfetSequencecontroller::getDirection() {
  return direction;
}













