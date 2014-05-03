#include "comDevice.h"

/*
 Wrapper class for Arduino library HardwareSerial 
 */

class usart: public comDevice {
public:

  usart(int baud) {
    Serial.begin(baud);
    fd_out = {0};
    fd_in = {0};

   //TODO INIT FUNCTION
   // fill in the UART file descriptor with pointer to writer.
   fdev_setup_stream (&fd_out, putChar, NULL, _FDEV_SETUP_WRITE);
   fdev_setup_stream (&fd_in, getChar, NULL, _FDEV_SETUP_READ);

   // The uart is the standard output device STDOUT.
   stdout = &fd_out ;
   stdin = &fd_in ;

  }

  char getChar() {
    return Serial.read();
  }
  int putChar(char c) {
    return Serial.write(c);
  }
  int available() {
    return Serial.available();
  }

};

