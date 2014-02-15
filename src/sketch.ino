
#include <avr/wdt.h>
#include <Arduino.h>
#include <WString.h>

#include "brushless.h"
#include "communicator.h"

#define F_CPU 8000000UL  //Uncomment for arduino Fio
//#define F_CPU 16000000UL  //Uncomment for arduino duemilanove

// Initialization of objects
brushless *brushlessPtr     = NULL;
communicator *serialCommPtr = NULL;

// Initialization of comman struct
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

  // Initialize serial communications
  if (serialCommPtr == NULL)
  {
    serialCommPtr = new communicator(); 
  }

  debug("SerialComm object initialized. ", 3);

   // Initialize brushless object
  if (brushlessPtr == NULL)
  {
    brushlessPtr  = new brushless();  
  }
  brushlessPtr->start(); //set prescaler and start the iteration
  brushlessPtr->startup(); //start-up ramp
  debug("Brushless object initialized. ", 3);
}

int commandExecute = 0;

void loop() { 

// Run main loop: check for serial command and set command 
  if(serialCommPtr->getHaveCommand() == 1){
	
	//	 communicator::logToSerial("Setting Command", 3);
    latestCommand = serialCommPtr->getCommand();
    debug("Received command type: " + String(latestCommand->type) + " value: " + String(latestCommand->value), 3);
    brushlessPtr->setCommand(latestCommand);
	commandExecute = 1;
  }

	brushlessPtr->iterate();

	if(commandExecute == 1){
    communicator::logToSerial(brushlessPtr->getResponse(), 3); //non dobbiamo usare la funzione debug ???!!!
	commandExecute = 0;
	}
}


// Register brushless object event handler to ISR for Timer 1
/*ISR(TIMER1_COMPB_vect) {*/
/*  brushlessPtr->eventHandler();*/

/*}*/

// Callback function for reserved Arduino keyword serial polling
void serialEvent(){
  serialCommPtr->eventHandler();
}

