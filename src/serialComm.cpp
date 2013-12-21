#include <Arduino.h>
#include "serialComm.h"


serialComm::serialComm(){

#ifdef DEBUG_ON  
  Serial.print("Entering constructor for: ");
  Serial.println(__FUNCTION__);
#endif


  bufferLength          = 0;
  
  inputBuffer           = "";
  inputBuffer.reserve(20);

  haveCommand           = 0;
  
  currentCommand        = (Command)malloc(sizeof(_command));
  currentCommand->type  = '\n';
  currentCommand->value = 0;

}

void serialComm::eventHandler() {
  
  char inChar = NULL;
  while (Serial.available()) {

    // get the new byte
    inChar         = (char)Serial.read(); 
    
    // concatenate to the  input string
    inputBuffer   += inChar;

    // if the incoming character is a newline, set a flag
    // extract integer value from string:
    if (inChar == '\n') {

      inChar       = NULL;
      bufferLength = inputBuffer.length();
      char inputStringValue[bufferLength-1];

      int i=1;
      while(i<bufferLength-1){
        inputStringValue[i-1] = inputBuffer[i];
        i++;
      }

      currentCommand->type  = inputBuffer[0];
      currentCommand->value = atoi(inputStringValue);

      haveCommand  = 1;
    } 
  }
}

Command serialComm::getCommand(){

  Command tmpCommand    = (Command)malloc(sizeof(_command));
  tmpCommand->type      = currentCommand->type;
  tmpCommand->value     = currentCommand->value;

  inputBuffer           = "";
  haveCommand           = 0;
  currentCommand->type  = '\n';
  currentCommand->value = 0;

  return tmpCommand;
}

int serialComm::getHaveCommand(){
  return haveCommand;
}

