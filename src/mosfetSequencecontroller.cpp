#include "mosfetSequencecontroller.h"

#include "atmegax8.h"

#define DEFAULT_INITIAL_RATE 100
#define DEFAULT_INITIAL_STATE 0

volatile int state;
int rate;
volatile int direction;


mosfetSequenceController::mosfetSequenceController(){
	state = DEFAULT_INITIAL_STATE;
  rate  = DEFAULT_INITIAL_RATE;
  direction = 0;
} 

  
  
  int mosfetSequenceController::init(){
		SET_AUTOMA_PORTD;
	  AUTOMA_ITERATE(state);
  }
	
  int mosfetSequenceController::iterate(){
		
			if(direction){           
					AUTOMA_ITERATE(++state % NUM_STATES); 
	    }
			else{
					AUTOMA_ITERATE((NUM_STATES-1 + state--) %NUM_STATES); 
	 		}
			return 0;
	}
  int mosfetSequenceController::setAutomaRate(int val){ rate = val; }
  int mosfetSequenceController::getAutomaRate(){ return rate; }

  int mosfetSequenceController::setDirection(int clockwise){ direction = clockwise; }
  int mosfetSequenceController::getDirection(){ return direction; }


