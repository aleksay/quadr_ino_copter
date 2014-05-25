
#include "config.h"  // include first !

#include "brushless.h"
#include "comMessage.h"
#include "comLogger.h"
#include "MsTimer2.h"
#include "comDeviceUSART.h"



// Initialization of objects
brushless brushless;

// Initialization of command struct
Command latestCommand;
void setup() {

  // undo arduino init() serial de-init nastiness
//#if defined(UCSRB)
//  UCSRB = 1;
//#elif defined(UCSR0B)
//  UCSR0B = 1;
//#endif

  //initialize uart
  uart_init(); // BAUD is 9600 by default
  stdio_init();

  printAndClearResetSource();

  //timer 2 init.
  // MsTimer2::set(50, brushlessPtr->incrementTime ); // Doesnt work
  MsTimer2::set(10, globalSetTime ); // 10ms period
  MsTimer2::start();
  debug("timer2 counter initialized");

}


// Run main loop
void loop() {
if(haveAsciiMessage==1) {
  debug("asciiMessage:%s ",asciiMessage);
  latestCommand = asciiString2Message(asciiMessage);
  haveAsciiMessage=0;
  brushless.setCommand(latestCommand);
}

  brushless.iterate();

}


void globalSetTime()
{
  brushless.incrementTime();
}


