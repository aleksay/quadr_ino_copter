#include "mosfetSequencecontroller.h"

#define DEFAULT_INITIAL_RATE 100



volatile int state;
//volatile int isRunning;
volatile int direction;



mosfetSequencecontroller::mosfetSequencecontroller() {
  //state     = DEFAULT_INITIAL_STATE;
  direction = 1;

}

//int mosfetSequencecontroller::getAutomaState(){
//  return states[state];
//}

int mosfetSequencecontroller::getState(){
  return state;
}

/* int mosfetSequencecontroller::start(){
    
	isRunning = 1;
	//debug("Running: %d" , isRunning);
}

int mosfetSequencecontroller::stop(){
	isRunning = 0;
} */

int mosfetSequencecontroller::setState(int _state){

	if(_state < 0 || _state >= NUM_STATES+1){
		
		
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
  
  if (direction) {
    state = ++state % NUM_STATES;
    AUTOMA_ITERATE(state);
  } 
  else {
    state = (--state +NUM_STATES)%NUM_STATES;
    //state = abs(NUM_STATES - 1 + --state) % NUM_STATES;
    AUTOMA_ITERATE(state);
  }
  //debug("%d",state);
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

void mosfetSequencecontroller::setOpenInverter() {
  AUTOMA_OPEN_INVERTER;
}











