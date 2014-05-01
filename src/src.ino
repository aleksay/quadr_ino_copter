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

// Flag for indicating if serial messages are available
int commandExecute = 0;

// Software reset
void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));
void wdt_init(void)
{
  MCUSR = 0;
  wdt_disable();
  return;
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



void globalSetTime()
{
  brushlessPtr->incrementTime();
}

void setup() {

  // Initialize serial communications
    
  debug("Entering SerialComm constructor", 5);
  if (serialCommPtr == NULL)
  {
    serialCommPtr = new communicator(); 
  }
  debug("SerialComm constructor DONE", 3);

  // Initialize brushless object
  debug("Entering Brushless constructor", 5);
  if (brushlessPtr == NULL)
  {
    brushlessPtr  = new brushless();  
  }
  debug("Brushless constructor DONE", 3);

  debug("Starting brushless", 5);
  brushlessPtr->start(); //set prescaler and start the iteration
  debug("brushlessPtr->start() DONE", 3);

  //timer 2 init.
  // MsTimer2::set(50, brushlessPtr->incrementTime ); // Doesnt work
  debug("Starting timer2 counter", 5);
  MsTimer2::set(10, globalSetTime ); // 10ms period
  MsTimer2::start();
  debug("MsTimer2::start() DONE", 3);



}



  // Run main loop
void loop() { 


  // check for serial command
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


