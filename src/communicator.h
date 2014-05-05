#ifndef COMUNICATOR_h
#define COMUNICATOR_h

#include <Arduino.h>
#include <WString.h>
#include "comLogger.h"
#include "comDeviceUSART.h"

typedef struct _command {
	char type;
	unsigned int value;
}*Command;

class communicator {

public:

	communicator();
	int 		 getHaveCommand();
	Command  getCommand();
        int emptyBuffer();
	void 		 eventHandler();
	void 		 iterate();
	
private:
	String inputBuffer;
	int haveCommand;
	Command currentCommand;
	int bufferLength;
};

#endif

