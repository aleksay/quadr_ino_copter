#ifndef COMUNICATOR_h
#define COMUNICATOR_h

#include <WString.h>

typedef struct _command{
  char type;
  unsigned int value;
}*Command;

class comunicator {

public:

  comunicator();
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

