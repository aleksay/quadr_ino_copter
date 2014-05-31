#ifndef mosfetSeqController_h
#define mosfetSeqController_h

#include "Arduino.h"
#include "avrUtils.h"
#include "comLogger.h"


const uint8_t states[NUM_STATES] = {	
  0b00001010,
  0b00001100,
  0b00010100,
  0b00010001,
  0b00100001,
  0b00100010
 };
class mosfetSequencecontroller {

public:

  mosfetSequencecontroller();

  int init();
  int getDirection();
  int setDirection(int clockwise);
  int commutePole();
/*   int start();
  int stop(); */
  void setOpenInverter();
  
  // int setAutomaRate(int val);
  //int getAutomaRate();
  //int getAutomaState();
  int getState();
  int setState(int _state);
  //int getAutomaState(int st);
private:

};

#endif


