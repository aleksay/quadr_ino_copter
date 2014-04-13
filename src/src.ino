#include <avr/wdt.h>
#include <avr/interrupt.h> 
#include <Arduino.h>
#include <WString.h>

#include "brushless.h"
#include "communicator.h"

#include "MsTimer2.h"



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

void globalSetTime()
{
  brushlessPtr->setTime();
}

void setup() {

  // Initialize serial communications
  if (serialCommPtr == NULL)
  {
    serialCommPtr = new communicator(); 
  }

  debug("SerialComm constructor has returned. ", 3);

  // Initialize brushless object
  if (brushlessPtr == NULL)
  {
    brushlessPtr  = new brushless();  
  }
  debug("Brushless constructor has returned. ", 3);

  brushlessPtr->start(); //set prescaler and start the iteration
  debug("brushlessPtr->start() has returned ", 3);

  //timer 2 init.
  // MsTimer2::set(50, brushlessPtr->setTime ); // Doesnt work
  MsTimer2::set(50, globalSetTime ); // 50ms period
  MsTimer2::start();
  debug("MsTimer2::start() has returned ", 3);



}

int commandExecute = 0;


void loop() { 


  // Run main loop: check for serial command and set command 
  if(serialCommPtr->getHaveCommand() == 1){

    latestCommand = serialCommPtr->getCommand();

    //here put a setCommand for each module in the sketch
    brushlessPtr->setCommand(latestCommand);

    //and then set flag for catching responses
    commandExecute = 1;
  }

  brushlessPtr->iterate();	

  if(commandExecute == 1){
    parseCommand(latestCommand);
    debug(brushlessPtr->getResponse(), 3); 
    //debug(" OCR1A: " + String (OCR1A) +" OCR1B:"+OCR1B ,3);
    commandExecute = 0;
  }
}


String parseCommand(Command command){

  switch(command->type){
  case 'R':
    wdt_init();
  default:
    return "";
  }	  
}

// Callback function for reserved Arduino keyword serial polling
void serialEvent(){
  serialCommPtr->eventHandler();
}


