
#include "config.h"  // include first !

#include "brushless.h"
#include "comMessage.h"
#include "comLogger.h"
#include "MsTimer2.h"
#include "comDeviceUSART.h"
#include "brushlessGlue.h"
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

  // init timers
  pwmInit();
  avrClockInit();

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

