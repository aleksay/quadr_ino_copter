#include <Arduino.h>
#include "comunicator.h"


comunicator::comunicator(){
  
//  Serial.print("Entering constructor for: ");
//  Serial.println(__FUNCTION__);

  bufferLength          = 0;
  
  inputBuffer           = "";
  inputBuffer.reserve(20);

  haveCommand           = 0;
  
  currentCommand        = (Command)malloc(sizeof(_command));
  currentCommand->type  = '\n';
  currentCommand->value = 0;

}

void comunicator::eventHandler() {
  
  char inChar = NULL;
  
  while (Serial.available()) {

    // get the new byte:
    inChar         = (char)Serial.read(); 

    // add it to the inputString:
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

Command comunicator::getCommand(){

  Command tmpCommand    = (Command)malloc(sizeof(_command));
  tmpCommand->type      = currentCommand->type;
  tmpCommand->value     = currentCommand->value;

  inputBuffer           = "";
  haveCommand           = 0;
  currentCommand->type  = '\n';
  currentCommand->value = 0;

  return tmpCommand;
}

int comunicator::getHaveCommand(){
  return haveCommand;
}

