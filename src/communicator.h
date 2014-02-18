#ifndef COMUNICATOR_h
#define COMUNICATOR_h

#include <Arduino.h>
#include <WString.h>

#define DEBUG 1
#define DEBUG_LEVEL 4 // change me to desired logging level

#ifdef DEBUG
#define debug(msg,prio) if(prio < DEBUG_LEVEL){													\
													Serial.print(String(__FILE__));								\
										    	Serial.print(":");														\
													Serial.print(String(__LINE__));								\
													Serial.print(" - ");													\
													Serial.println(msg);													\
												}
#else
#define debug(msg)
#endif


static int logLevel = 4;


typedef struct _command {
	char 				type;
	unsigned int value;
}*Command;

class communicator {

public:

	communicator();
	int 				getHaveCommand();
	Command 		getCommand();
	void 				eventHandler();
	void 				iterate();
	//static int logToSerial(String logString, int logPriority);
	//static int printToSerial(String logString);

private:
	String	inputBuffer;
	int 		haveCommand;
	Command currentCommand;
	int     bufferLength;
};

#endif

