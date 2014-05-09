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

// Initialization of command struct
Command latestCommand;

void setup() {

  //initialize uart
  uart_init(BAUD); // BAUD is 9600 by default

  // Initialize serial parser    
  if (serialCommPtr == NULL)
  {
    serialCommPtr = new communicator(); 
  }
  debug("SerialComm constructor returned");

  // Initialize brushless object
  if (brushlessPtr == NULL)
  {
    brushlessPtr  = new brushless();  
  }
  debug("Brushless constructor returned");

//  debug("Starting brushless");
//  brushlessPtr->start();
//  debug("Starting brushless DONE");

  //timer 2 init.
  // MsTimer2::set(50, brushlessPtr->incrementTime ); // Doesnt work
  MsTimer2::set(10, globalSetTime ); // 10ms period
  MsTimer2::start();
  debug("timer2 counter initialized");



}



  // Run main loop
void loop() { 

  // check for serial command
  if(serialCommPtr->getHaveCommand() == 1){

    latestCommand = serialCommPtr->getCommand();

    //here put a setCommand for each module in the sketch
    brushlessPtr->setCommand(latestCommand);
    //debug("Received latestCommand->type:%c",latestCommand->type);
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


