#include "communicator.h"


communicator::communicator() {
  haveCommand           = 0;

  currentCommand        = (Command) malloc(sizeof(_command));
  currentCommand->type  = '\n';
  currentCommand->value = 0;
}


void communicator::asciiString2Message( char * asciiMessage ){
      int len = strlen(asciiMessage);
      char inputStringValue[len - 1];

      int i = 1;
      while (i < (len - 1) ) {
        inputStringValue[i - 1] = asciiMessage[i];
        i++;
      }
      currentCommand->type = asciiMessage[0];
      currentCommand->value = strtol(inputStringValue, NULL, 0);

debug("command received:%c , Value:%d",currentCommand->type,currentCommand->value);

}


//void communicator::eventHandler() {
//debug("event");
//
//  char inChar = NULL;
//
// // while (uart_available() ) {
////    debug("available");
//    // get the new byte
//      loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
//    inChar = (char) UDR0;
//
//    // concatenate to the  input string
//    inputBuffer[strlen(inputBuffer)] = inChar; /* copy the string as last */
//
//    // if the incoming character is a newline, set a flag
//    // extract integer value from string:
//    if (inChar == '\n') {
//
//      inChar = NULL;
//      bufferLength = strlen(inputBuffer);
//      char inputStringValue[bufferLength - 1];
//
//      int i = 1;
//      while (i < bufferLength - 1) {
//        inputStringValue[i - 1] = inputBuffer[i];
//        i++;
//      }
//      currentCommand->type = inputBuffer[0];
//      currentCommand->value = strtol(inputStringValue, NULL, 0);
//
//      haveCommand = 1;
//  //  }
//  }
//}

Command communicator::getCommand() {

  Command tmpCommand    = (Command) malloc(sizeof(_command));
  tmpCommand->type      = currentCommand->type;
  tmpCommand->value     = currentCommand->value;

  haveCommand           = 0;
  currentCommand->type  = '\n';
  currentCommand->value = 0;

  return tmpCommand;
}

int communicator::getHaveCommand() {
  return haveCommand;
}



