#include "avrBrushlessPins.h"




volatile int pins_state;
volatile int pins_direction= 1;


int pins_getState(){
  return pins_state;
}


int pins_setState(int _state){

	if(_state < 0 || _state >= PINS_NUM_STATES+1){
		
		
		return -1;
	}

	pins_state =_state;
	AUTOMA_ITERATE(pins_state);

	return 0;
}

int pins_init() {

  AUTOMA_PIN_INIT;
  pins_setState(0);
}

void pins_commutePole(void) {
  
  if (pins_direction) {
    pins_state = ++pins_state % PINS_NUM_STATES;
    AUTOMA_ITERATE(pins_state);
  } 
  else {
    pins_state = (--pins_state + PINS_NUM_STATES)%PINS_NUM_STATES;
    AUTOMA_ITERATE(pins_state);
  }
}


int pins_setDirection(int clockwise) {
  pins_direction = clockwise;
}
int pins_getDirection() {
  return pins_direction;
}

void pins_setOpenInverter() {
  AUTOMA_OPEN_INVERTER;
}











