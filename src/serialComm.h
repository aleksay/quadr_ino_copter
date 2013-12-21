#ifndef SERIALCOMM_h
#define SERIALCOMM_h

#include <WString.h>

typedef struct _command
{
  char type;
  unsigned int value;
} *Command;

class serialComm {

public:

  serialComm();
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

