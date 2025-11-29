#ifndef COMUNICATOR_H
#define COMUNICATOR_H

#ifdef __AVR__

#include <avrUsart.h>

#endif

#include <ctype.h>
#include <stdlib.h>

#include <logger.h>

#define DEFAULT_COMMAND_MAP_SIZE 100
#define MAX_RAW_MESSAGE_SIZE 6

typedef struct _commandEntry {
  char key;
  void (*handler)(uint8_t args);
} *CommandEntry;

typedef struct _command {
  char type;
  uint8_t value;
} *Command;

class comunicator {

public:
  comunicator();
  comunicator(CommandEntry *_commandMap, int8_t _commandMapSize);
  comunicator(char *commands, void (**handlers)(uint8_t),
              int8_t commandMapSize);
  ~comunicator();

  void setCommandMap(CommandEntry *_commandMap, int8_t commandMapSize);
  void read(); // main iteration loop

private:
  void runCommand(Command _recivedCommand);
  int8_t isCommandRegistered(Command _recivedCommand);

  Command readCommand();

  // variables
  CommandEntry *commandMap;
  int8_t commandMapSize;
};

#endif
