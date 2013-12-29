
#include <avr/wdt.h>
#include <Arduino.h>
#include <WString.h>

#include "brushless.h"
#include "serialComm.h"

#define F_CPU 8000000UL  //Uncomment for arduino Fio
//#define F_CPU 16000000UL  //Uncomment for arduino duemilanove

// Initialization of objects
brushless *brushlessPtr   = NULL;
serialComm *serialCommPtr = NULL;
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


  serialComm::logToSerial("Serial initialized. " , 3);


   // Initialize serial communications
  if (serialCommPtr == NULL)
  {
    serialCommPtr = new serialComm(); 
  }

  serialComm::logToSerial("SerialComm object initialized. ", 3);


   // Initialize brushless communications
  if (brushlessPtr == NULL)
  {
    brushlessPtr  = new brushless();
  }
  brushlessPtr->startup();

  serialComm::logToSerial("Brushless object initialized. ", 3);



}

void loop() { 

// Run main loop: check for serial command and set command 
  if(serialCommPtr->getHaveCommand() == 1){
    latestCommand = serialCommPtr->getCommand();
    setCommand(latestCommand);
  }
}

void setCommand(Command command){


  char numstr[21]; // for holding temporary string  
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
	serialComm::printToSerial ( "--QUERY--");
	serialComm::printToSerial( String("f") + itoa(brushlessPtr->getFrequency(), numstr, 10 )  );
	serialComm::printToSerial( String("d") + itoa(brushlessPtr->getDuty(), numstr, 10 )  );
	serialComm::printToSerial( String("r") + itoa(brushlessPtr->getRefreshRate(), numstr, 10 )  );
	serialComm::printToSerial ( "----"); 
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





