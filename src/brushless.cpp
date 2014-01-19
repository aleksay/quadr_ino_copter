/*

 Il circuito prevede che i pin 2 3 e 4 siano collegati ai
 pin di comando dei buffer gate il cui input e' il segnale pwm generato dal timer.
 I 3 gate vanno connessi ai mosfet nella parte superiore.
 I pin 5,6 e 7 devono essere invece collegati direttamente ai mosfet della parte inferiore.

 */

#include "brushless.h"

#define NUM_STATES 6

#define RAMP_INIT_FREQUENCY 260
#define RAMP_INIT_DUTY 245
#define RAMP_INIT_REFREASHRATE 350

#define RAMP_FIN_FREQUENCY 260
#define RAMP_FIN_DUTY 150
#define RAMP_FIN_REFREASHRATE 26

timer *timer1_pwm = NULL;
mosfetSequencecontroller * automa = NULL;

startupData freqData;
startupData dutyData;
startupData refreshData;

brushless::brushless() {

	timer1_pwm = new timer();
	automa = new mosfetSequencecontroller();

	timer1_pwm->setFrequency(RAMP_INIT_FREQUENCY);
	timer1_pwm->setDuty(RAMP_INIT_DUTY);

	automa->setAutomaRate(RAMP_INIT_REFREASHRATE);

	timer1_pwm->start();
	automa->init();

	//cpmCounter = 0;

}

void brushless::startupcalc(startupData valueData, int slow) {
	//start_values ritorno;
	int delta = valueData->currentValue - valueData->end;
	float minus = delta * valueData->decrement;
	if (minus >= 1) {
		valueData->currentValue = valueData->currentValue - floor(minus);
	} else {
		if (slow == 1) {
			valueData->resto = valueData->resto + minus;
			if (valueData->resto >= 1) {
				valueData->currentValue = valueData->currentValue	- floor(valueData->resto);
				valueData->resto = valueData->resto - floor(valueData->resto);
			}

		} else if (slow == 0) {
			valueData->currentValue = valueData->currentValue - 1;
		}
	}
}


int brushless::startup() {

	communicator::logToSerial(String("Entering brushless::") + __func__, 5);

	freqData = (startupData) malloc(sizeof(_startup_data));
	freqData->start = RAMP_INIT_FREQUENCY;
	freqData->end = RAMP_FIN_FREQUENCY;
	freqData->decrement = 0.08;
	freqData->currentValue = freqData->start;
	freqData->resto = 0;

	dutyData = (startupData) malloc(sizeof(_startup_data));
	dutyData->start = RAMP_INIT_DUTY;
	dutyData->end = RAMP_FIN_DUTY;
	dutyData->decrement = 0.1;
	dutyData->currentValue = dutyData->start;
	dutyData->resto = 0;

  refreshData = (startupData) malloc(sizeof(_startup_data));
	refreshData->start = RAMP_INIT_REFREASHRATE;
	refreshData->end = RAMP_FIN_REFREASHRATE;
	refreshData->decrement = 0.2;
	refreshData->currentValue = refreshData->start;
	refreshData->resto = 0;

	startupping = 1;

}



void brushless::iterate() {

	//if startup mode do startup

	if (startupping) {
		if ((freqData->currentValue > freqData->end)
				|| (dutyData->currentValue > dutyData->end)
				|| (refreshData->currentValue > refreshData->end)) {

			if (freqData->currentValue > freqData->end) {
				startupcalc(freqData, 1);
				timer1_pwm->setFrequency(freqData->currentValue);
			}

			if (dutyData->currentValue > dutyData->end) {
				startupcalc(dutyData, 1);
				timer1_pwm->setDuty(dutyData->currentValue);
			}

			if (refreshData->currentValue > refreshData->end) {
				startupcalc(refreshData, 1);
				automa->setAutomaRate(refreshData->currentValue);
			}

			delay(50);
		} else {
			startupping = 0;
		}

	}
	// else parse latestcommand
	else if (!commandRead) {
			parseCommand(latestCommand);
			commandRead = 1;
	}
}


String brushless::parseCommand(Command command){
	
  int r = -10;

  switch(command->type){

  case 'f':
    return String(timer1_pwm->setFrequency(command->value));
  case 'd':
    return String(timer1_pwm->setDuty(command->value));
  case 'r':
    return String(automa->setAutomaRate(command->value));
    
  case 'p':
		return String( "--QUERY--\n") +
		 			 String("f") + String(timer1_pwm->getFrequency())   + String("\n") +
           String("d") + String(timer1_pwm->getDuty())        + String("\n") +
           String("r") + String(automa->getAutomaRate()) + String("\n") +
		       String( "----"); 
    break;

  default:
  break;  
  }
}

int brushless::setCommand(Command command) {
	latestCommand = command;
	commandRead = 0;
}
String brushless::getResponse() {
	return latestMessage;
}



