#ifndef COMUNICATOR_h
#define COMUNICATOR_h

#include <Arduino.h>
#include <string.h>
#include "comLogger.h"
//#include "comDeviceUSART.h"

typedef struct _command {
	char type;
	unsigned int value;
}*Command;

class communicator {

public:

	communicator();
	int 		 getHaveCommand();
	Command  getCommand();
	void 		 iterate();
	
private:
	int haveCommand;
	Command currentCommand;
};

#endif

