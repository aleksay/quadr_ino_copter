#ifndef COMUNICATOR_h
#define COMUNICATOR_h

#include <Arduino.h>
#include <WString.h>

static int logLevel = 4; // change me to desired logging level

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
	static int logToSerial(String logString, int logPriority);
	static int printToSerial(String logString);

private:
	String inputBuffer;
	int haveCommand;
	Command currentCommand;
	int bufferLength;
};

#endif

