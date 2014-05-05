#include "config.h"  // include first !

#include <Arduino.h>
#include <WString.h>

#include "brushless.h"
#include "communicator.h"
#include "comLogger.h"
#include "comDeviceUSART.h"
#include "MsTimer2.h"




// Initialization of objects
brushless *brushlessPtr     = NULL;
communicator *serialCommPtr = NULL;


void setup() {

  //initialize uart
  uart_init(BAUD); // BAUD is 9600 by default

  // Initialize serial parser    
  debug("Entering SerialComm constructor");
  if (serialCommPtr == NULL)
  {
    serialCommPtr = new communicator(); 
  }
  debug("SerialComm constructor DONE");

  // Initialize brushless object
  debug("Entering Brushless constructor");
  if (brushlessPtr == NULL)
  {
    brushlessPtr  = new brushless();  
  }
  debug("Brushless constructor DONE");

  debug("Starting brushless");
  brushlessPtr->start();
  debug("Starting brushless DONE");

  //timer 2 init.
  // MsTimer2::set(50, brushlessPtr->incrementTime ); // Doesnt work
  debug("Starting timer2 counter");
  MsTimer2::set(10, globalSetTime ); // 10ms period
  MsTimer2::start();
  debug("Starting timer2 counter DONE");



}



  // Run main loop
void loop() { 

  // check for serial command
  if(serialCommPtr->getHaveCommand() == 1){
    brushlessPtr->setCommand( serialCommPtr->getCommand() );
    serialCommPtr->emptyBuffer();
    
  }
  brushlessPtr->iterate();	
}



// Callback function for reserved Arduino keyword serial polling
void serialEvent()
{
  serialCommPtr->eventHandler();
}



void globalSetTime()
{
  brushlessPtr->incrementTime();
}


