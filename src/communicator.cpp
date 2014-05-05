#include "communicator.h"


communicator::communicator() {


  bufferLength          = 0;

  inputBuffer           = "";
  inputBuffer.reserve(20);

  haveCommand           = 0;

  currentCommand        = (Command) malloc(sizeof(_command));
  currentCommand->type  = '\n';
  currentCommand->value = 0;
}

void communicator::eventHandler() {

  char inChar = NULL;

  while (uart_available() ) {

    // get the new byte
    inChar = (char) Serial.read();

    // concatenate to the  input string
    inputBuffer += inChar;

    // if the incoming character is a newline, set a flag
    // extract integer value from string:
    if (inChar == '\n') {

      inChar = NULL;
      bufferLength = inputBuffer.length();
      char inputStringValue[bufferLength - 1];

      int i = 1;
      while (i < bufferLength - 1) {
        inputStringValue[i - 1] = inputBuffer[i];
        i++;
      }
      currentCommand->type = inputBuffer[0];
      currentCommand->value = strtol(inputStringValue,NULL,0);

      haveCommand = 1;
    }
  }
}

Command communicator::getCommand() {
  return currentCommand;
}

int communicator::emptyBuffer() {

  inputBuffer           = "";
  haveCommand           = 0;
  currentCommand->type  = '\n';
  currentCommand->value = 0;

  return 0;
}


int communicator::getHaveCommand() {
  return haveCommand;
}



