#ifndef COMUNICATOR_h
#define COMUNICATOR_h

#include <Arduino.h>
#include <string.h>
#include "comLogger.h"


typedef struct _command {
  char type;
  unsigned int value;
}
*Command;

Command asciiString2Message(char * asciiMessage);


#endif


