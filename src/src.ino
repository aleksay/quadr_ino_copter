
#include "config.h"  // include first !

#include "brushless.h"
#include "comMessage.h"
#include "comLogger.h"
#include "MsTimer2.h"
#include "comDeviceUSART.h"
#include "avrTime.h"


// Class declarations
brushless brushless;

// Struct declarations
Command latestCommand;

void setup() {

  //initialize uart
  uart_init();
  stdio_init();

  printAndClearResetSource(); // TODO broken

  // init clock
  avrClockInit();
  
  //initialize brushless devices
  brushless.init();

  debug("setup finished");
}


// Run main loop
void loop() {
  if (haveAsciiMessage == 1) {
    debug("asciiMessage:%s ", asciiMessage);
    latestCommand = asciiString2Message(asciiMessage);
    haveAsciiMessage = 0;
    brushless.setCommand(latestCommand);
  }

  brushless.iterate();

}

