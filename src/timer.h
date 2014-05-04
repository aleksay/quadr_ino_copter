#ifndef Timer
#define Timer

#include "atmegax8.h"
#include "comLogger.h"

class timer {

public:

  virtual int start() = 0;
  virtual int start(int prescaler) = 0;
  virtual int setFrequency(unsigned int val) = 0;
  virtual int setDuty(int val) = 0;
  virtual int setPrescaler(int val) = 0;
  virtual unsigned int getFrequency() = 0;
  virtual int getDuty() = 0;
  virtual int getPrescaler() = 0;
  //int stop(); not working with prescaler

private:


};
#endif

