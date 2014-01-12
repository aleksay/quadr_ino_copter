#include "mosfetSequencecontroler.h"

#include "atmegax8.h"

#define DEFAULT_INITIAL_RATE 100
#define DEFAULT_INITIAL_STATE 0

volatile int state;
int rate;
volatile int direction;


mosfetSequencecontroler::mosfetSequenceControler(){
	state = DEFAULT_INITIAL_STATE;
  rate  = DEFAULT_INITIAL_RATE;
  direction = 0;
} 

  
  
  int mosfetSequencecontroler::init(){
		SET_AUTOMA_PORTD;
	  AUTOMA_ITERATE(state);
  }
	
  int mosfetSequencecontroler::iterate(){
		
			if(direction){           
					AUTOMA_ITERATE(++state % NUM_STATES); 
	    }
			else{
					AUTOMA_ITERATE((NUM_STATES-1 + state--) %NUM_STATES); 
	 		}
			return 0;
	}
  int mosfetSequencecontroler::setAutomaRate(int val){ rate = val; }
  int mosfetSequencecontroler::getAutomaRate(){ return rate; }

  int mosfetSequencecontroler::setDirection(int clockwise){ direction = clockwise; }
  int mosfetSequencecontroler::getDirection(){ return direction; }


