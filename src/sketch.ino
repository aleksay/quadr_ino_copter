
#include <avr/wdt.h>
#include <Arduino.h>
#include <WString.h>

#include "brushless.h"
#include "communicator.h"

#define F_CPU 8000000UL  //Uncomment for arduino Fio
//#define F_CPU 16000000UL  //Uncomment for arduino duemilanove

// Initialization of objects
brushless *brushlessPtr   = NULL;
communicator *serialCommPtr = NULL;
// Initialization of struct
Command latestCommand;

// Software reset 
void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));
void wdt_init(void)
{
    MCUSR = 0;
    wdt_disable();
    return;
}


void setup() {

  Serial.begin(9600);


  communicator::logToSerial("Serial initialized. " , 3);


   // Initialize serial communications
  if (serialCommPtr == NULL)
  {
    serialCommPtr = new communicator(); 
  }

  communicator::logToSerial("SerialComm object initialized. ", 3);


   // Initialize brushless communications
  if (brushlessPtr == NULL)
  {
    brushlessPtr  = new brushless();  // This is critical  - create a new class here only
  }
  brushlessPtr->startup();

  communicator::logToSerial("Brushless object initialized. ", 3);



}

void loop() { 

// Run main loop: check for serial command and set command 
  if(serialCommPtr->getHaveCommand() == 1){
    latestCommand = serialCommPtr->getCommand();
    setCommand(latestCommand);
  }
}

void setCommand(Command command){


  
  int r = -10;

  switch(command->type){

  case 'f':
    r = -10;
    r = brushlessPtr->setFrequency(command->value);
    break;

  case 'd':
    r = -10;
    r = brushlessPtr->setDuty(command->value);
    break;

  case 'r':
    r = -10;
    r = brushlessPtr->setRefreshRate(command->value);
    break;
    

  case 'p':
	communicator::printToSerial ( "--QUERY--");
	communicator::printToSerial( String("f") + String(brushlessPtr->getFrequency())  );
	communicator::printToSerial( String("d") + String(brushlessPtr->getDuty())  );
	communicator::printToSerial( String("r") + String(brushlessPtr->getRefreshRate())  );
	communicator::printToSerial ( "----"); 
    break;

  case 'R':
    wdt_init();
    break;

  default:
  break;  
  }
}


// Register brushless object event handler to ISR for Timer 1
ISR(TIMER1_COMPB_vect) {
  brushlessPtr->eventHandler();

}

// Callback function for reserved Arduino keyword serial polling
void serialEvent(){
  serialCommPtr->eventHandler();
}





