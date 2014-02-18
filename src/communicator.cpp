#include "communicator.h"

#include "usart.cpp"


usart* usart_module = NULL;

communicator::communicator() {

	bufferLength 					= 0;

	inputBuffer 					= "";
	inputBuffer.reserve(20);

	haveCommand 					= 0;

	currentCommand 				= (Command) malloc(sizeof(_command));
	currentCommand->type 	= '\n';
	currentCommand->value = 0;

	usart_module 					= new usart(9600);
	
  debug(String("Entering constructor for: ") + __func__,3)

	if (usart_module != NULL)
		debug("Serial initialized. ", 3);
}

void communicator::eventHandler() {

	char inChar = NULL;

	while (usart_module->available()) {

		// get the new byte
		inChar 			 						= (char) usart_module->read();

		// concatenate to the  input string
		inputBuffer 				 	 += inChar;

		// if the incoming character is a newline, set a flag
		// extract integer value from string:
		if (inChar == '\n') {

			inChar 		 						= NULL;
			bufferLength 					= inputBuffer.length();
			char inputStringValue[bufferLength - 1];

			int i 								= 1;
			while (i < bufferLength - 1) {
				inputStringValue[i - 1] = inputBuffer[i];
				i++;
			}

			currentCommand->type  = inputBuffer[0];
			currentCommand->value = atoi(inputStringValue);

			haveCommand = 1;
		}
	}
}

Command communicator::getCommand() {

	Command tmpCommand 		= (Command) malloc(sizeof(_command));
	tmpCommand->type 			= currentCommand->type;
	tmpCommand->value 		= currentCommand->value;

	inputBuffer 					= "";
	haveCommand 					= 0;
	currentCommand->type  = '\n';
	currentCommand->value = 0;

	return tmpCommand;
}

int communicator::getHaveCommand() {
	return haveCommand;
}

//DEPRECATION
int logToSerial(String logString, int logPriority) {
//int communicator::logToSerial(String logString, int logPriority) {

	if (logPriority < logLevel) {
		Serial.println(logString);
	}

	return 0;
}

//DEPRECATION
int printToSerial(String logString) {
//int communicator::printToSerial(String logString) {

	Serial.println(logString);
	return 0;

}

