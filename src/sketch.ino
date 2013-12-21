
#include <avr/wdt.h>

#include "brushless.h"
#include "serialComm.h"


#define F_CPU 16000000UL

#ifdef DEBUG
#define DEBUG_ON
#endif


brushless *brushlessPtr   = NULL;
serialComm *serialCommPtr = NULL;
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

#ifdef DEBUG_ON
  Serial.println("Serial initialized. ");
#endif

   // Initialize serial communications
  if (serialCommPtr == NULL)
  {
    serialCommPtr = new serialComm(); 
  }
#ifdef DEBUG_ON
  Serial.println("SerialComm object initialized. ");
#endif

   // Initialize brushless communications
  if (brushlessPtr == NULL)
  {
    brushlessPtr  = new brushless();
  }
  brushlessPtr->startup();
#ifdef DEBUG_ON
  Serial.println("Brushless object initialized. ");
#endif


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


// Register brushless object event handler to ISR for Timer 1
ISR(TIMER1_COMPB_vect) {
  brushlessPtr->eventHandler();
}

// Callback function for reserved Arduino keyword serial polling
void serialEvent(){
  serialCommPtr->eventHandler();
}





