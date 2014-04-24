#ifndef COMUNICATOR_h
#define COMUNICATOR_h

#include <Arduino.h>
#include <WString.h>

#define DEBUG 1
#define DEBUG_LEVEL 4 // change me to desired logging level
#ifdef DEBUG

#define FILE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define debug(msg,prio) if(prio < DEBUG_LEVEL){			\
				Serial.print(String(FILE));	\
			    	Serial.print(":");		\
				Serial.print(String(__LINE__));	\
				Serial.print(" - ");		\
				Serial.print(String(__func__));	\
				Serial.print("() - ");		\
				Serial.println(msg);		\
			}
#else
#define debug(msg)
#endif

static int logLevel = 4;

typedef struct _command {
	char type;
	unsigned int value;
}*Command;

class communicator {

public:

	communicator();
	int getHaveCommand();
	Command getCommand();
	void eventHandler();
	void iterate();

private:
	String inputBuffer;
	int haveCommand;
	Command currentCommand;
	int bufferLength;
};

#endif

