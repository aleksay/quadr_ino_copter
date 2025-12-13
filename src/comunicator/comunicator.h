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

#define DEFAULT_BAUDRATE 9600

typedef struct _commandEntry
{
  char key;
  void (*handler) (uint8_t args);
} *CommandEntry;

typedef struct _command
{
  char type;
  uint8_t value;
} *Command;

class comunicator
{

public:
  comunicator (uint16_t _baud = DEFAULT_BAUDRATE);
  comunicator (CommandEntry *_commandMap, int8_t _commandMapSize,
               uint16_t _baud = DEFAULT_BAUDRATE);
  comunicator (char *commands, void (**handlers) (uint8_t),
               int8_t commandMapSize, uint16_t _baud = DEFAULT_BAUDRATE);
  ~comunicator ();

  void setCommandMap (CommandEntry *_commandMap, int8_t commandMapSize);
  void read (); // main iteration loop

private:
  CommandEntry *commandMap;
  Command lastCommand;
  int8_t commandMapSize;

  Command readCommand ();
  void runCommand (Command _recivedCommand);
  int8_t isCommandRegistered (Command _recivedCommand);
  // variables
  
  
};

#endif
