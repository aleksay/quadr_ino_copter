
#include <avr/wdt.h>

#include "brushless.h"
#include "serialComm.h"


#define F_CPU 16000000UL

brushless *brushlessPtr   = NULL;
serialComm *serialCommPtr = NULL;
Command lastCommand;


void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));

void wdt_init(void)
{
    MCUSR = 0;
    wdt_disable();

    return;
}


void setup() {

  Serial.begin(9600);
  Serial.println("init ");
  if (serialCommPtr == NULL)
  {
    serialCommPtr = new serialComm();  // This is critical  - create a new class here only
  }
  if (brushlessPtr == NULL)
  {
    brushlessPtr  = new brushless();  // This is critical  - create a new class here only
  }
  
  Serial.println("init stop");
}

void loop() { 

  if(serialCommPtr->getHaveCommand() == 1){

    lastCommand = serialCommPtr->getCommand();    
    commandMap(lastCommand);
  }
}

void commandMap(Command currentCommand){
  
  int r = -10;               //return value holder

  switch(currentCommand->type){

  case 'f':
    r = -10;
    r = brushlessPtr->setFrequency(currentCommand->value);
    break;

  case 'd':
    r = -10;
    r = brushlessPtr->setDuty(currentCommand->value);
    break;

  case 'r':
    r = -10;
    r = brushlessPtr->setRefreshRate(currentCommand->value);
    break;
    

  case 'p':
    Serial.print("f");
    Serial.println(brushlessPtr->getFrequency());
    Serial.print("d");
    Serial.println(brushlessPtr->getDuty());
    Serial.print("r");
    Serial.println(brushlessPtr->getRefreshRate());
    break;

  case 'R':
    wdt_init();
    break;

  default:
  break;  
  }
}

ISR(TIMER1_COMPB_vect) {
  brushlessPtr->eventHandler();

}

void serialEvent(){
  serialCommPtr->eventHandler();
}





