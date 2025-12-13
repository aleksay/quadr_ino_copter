#include <comunicator.h>

comunicator::comunicator (uint16_t _baud)
{

  commandMap = NULL;
  commandMapSize = DEFAULT_COMMAND_MAP_SIZE;
  lastCommand = (Command)malloc (sizeof (_command));

#ifdef AVRUSART_H
  uart_init (_baud);

  stdin = &uart_stream;
  stdout = &uart_stream;

#endif
}

comunicator::comunicator (CommandEntry *_commandMap, int8_t _commandMapSize,
                          uint16_t _baud)
    : comunicator (_baud)
{
  // #ifdef AVRUSART_H
  //   uart_init(DEFAULT_BAUDRATE);

  //  stdin = &uart_stream;
  //  stdout = &uart_stream;

  // #endif

  commandMap = _commandMap;
  commandMapSize = _commandMapSize;
}

comunicator::comunicator (char *_commands, void (**_handlers) (uint8_t),
                          int8_t _commandMapSize, uint16_t _baud)
    : comunicator (_baud)
{
  // #ifdef AVRUSART_H
  //   uart_init(DEFAULT_BAUDRATE);

  //  stdin = &uart_stream;
  //  stdout = &uart_stream;

  // #endif

  int i = 0;
  CommandEntry _commandMap[_commandMapSize];

  for (; i < _commandMapSize; i++)
    {
      _commandMap[i] = (CommandEntry)malloc (sizeof (_commandEntry));
      _commandMap[i]->key = _commands[i];
      _commandMap[i]->handler = _handlers[i];
    }

  commandMap = _commandMap;
  commandMapSize = _commandMapSize;
}

comunicator::~comunicator () {}

void
comunicator::setCommandMap (CommandEntry *_commandMap, int8_t _commandMapSize)
{
  commandMap = _commandMap;
  commandMapSize = _commandMapSize;

  //	int i=0;
  //	for(;i<commandMapSize;i++){
  //		log_info("Element number %d, key: %c",i,commandMap[i]->key);
  //	}
}

void
comunicator::read ()
{

  Command recivedCommand = readCommand ();

  if (recivedCommand != NULL)
    {
      runCommand (recivedCommand);
    }
}

Command
comunicator::readCommand ()
{

  char read_buffer[MAX_RAW_MESSAGE_SIZE];
  char inputStringValue[MAX_RAW_MESSAGE_SIZE - 1];
  uint16_t i = 1;
  //Command recivedCommand;

  if (fgets (read_buffer, sizeof (read_buffer), stdin) == NULL)
    {

#ifdef AVRUSART_H
      uart_flush_rx_buffer ();
#endif
      return NULL;
    }

#ifdef AVRUSART_H
  uart_flush_rx_buffer ();
#endif

  if (!isalpha (read_buffer[0]) || !isspace (read_buffer[1])
      || !isdigit (read_buffer[2]))
    {
      return NULL;
    }

  while (i < sizeof (read_buffer))
    {
      inputStringValue[i - 1] = read_buffer[i];
      i++;
    }

  //recivedCommand = (Command)malloc (sizeof (_command));
  //recivedCommand->type = read_buffer[0];
  //recivedCommand->value = strtol (inputStringValue, NULL, 0);
  //return recivedCommand;
  lastCommand->type = read_buffer[0];
  lastCommand->value = strtol (inputStringValue, NULL, 0);
  return lastCommand;
  
}

void
comunicator::runCommand (Command _recivedCommand)
{

  int8_t i = isCommandRegistered (_recivedCommand);
  if (i < DEFAULT_COMMAND_MAP_SIZE)
    {
      commandMap[i]->handler (_recivedCommand->value);
    }
}

int8_t
comunicator::isCommandRegistered (Command _recivedCommand)
{

  for (int8_t i = 0; i < commandMapSize; i++)
    {

      if (_recivedCommand->type == commandMap[i]->key)
        {
          return i;
        }
    }
  return DEFAULT_COMMAND_MAP_SIZE;
}
