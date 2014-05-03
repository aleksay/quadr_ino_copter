#ifndef COMDEVICE_h
#define COMDEVICE_h

#include <Arduino.h>
#include <stdio.h>

class comDevice {

public:
static FILE fd_in;
static FILE fd_out;

  comDevice() {
  }
  virtual char getChar()       = 0;
  virtual int putChar(char c) = 0;
  virtual int available()   = 0;

private:

};

#endif


