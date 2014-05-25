#include "comMessage.h"



Command asciiString2Message( char * asciiMessage ){
  int len = strlen(asciiMessage);
  char inputStringValue[len - 1];

  int i = 1;
  while (i < len) {
    inputStringValue[i - 1] = asciiMessage[i];
    i++;
  }
  Command currentCommand = (Command) malloc(sizeof(_command));
  currentCommand->type = asciiMessage[0];
  currentCommand->value = strtol(inputStringValue, NULL, 0);

  debug("command received:%c , Value:%d",currentCommand->type,currentCommand->value);
  return currentCommand;
}





