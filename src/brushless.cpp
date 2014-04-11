

#include "brushless.h"
#include "timer1.cpp"
#include "timer0.cpp"
#include "timer.h"

timer1 *timer1_pwm = NULL;
timer0 *timer0_pwm = NULL;
mosfetSequencecontroller * automa = NULL;

startupData freqData;
startupData dutyData;


brushless::brushless() {

  
  	startupping = 0;
	commandRead = 0;
  
	timer1_pwm   = new timer1();
	
	timer0_pwm   = new timer0();

	automa	     = new mosfetSequencecontroller();
	//automa->setAutomaRate(RAMP_INIT_REFREASHRATE);
	automa->init();

	latestCommand = (Command)malloc(sizeof(_command));
	latestCommand->type = 'n';

	timer0_pwm->start(0);
}

void brushless::startupcalc(startupData valueData, int slow) {
	//start_values ritorno;
	unsigned int delta = valueData->currentValue - valueData->end;
	
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

	debug(String("Entering brushless::") + __func__, 5);

	freqData = (startupData) malloc(sizeof(_startup_data));
	freqData->start = timer1_pwm->getFrequency();
	freqData->end = RAMP_FIN_FREQUENCY_T1;
	freqData->decrement = 0.003;
	freqData->currentValue = freqData->start;
	freqData->resto = 0;

	dutyData = (startupData) malloc(sizeof(_startup_data));
	dutyData->start = timer0_pwm->getDuty();
	dutyData->end = RAMP_FIN_DUTY_T0;
	dutyData->decrement = 0.01;
	dutyData->currentValue = dutyData->start;
	dutyData->resto = 0;

	startupping = 1;

}

void brushless::iterate() {

	//if startup mode do startup

	if (startupping) {
		
		if ((freqData->currentValue > freqData->end)
		||  (dutyData->currentValue > dutyData->end)) {

			if (freqData->currentValue > freqData->end) {

				startupcalc(freqData, 1);
				timer1_pwm->setFrequency(freqData->currentValue);
			}

			if (dutyData->currentValue > dutyData->end) {
				startupcalc(dutyData, 1);
				timer0_pwm->setDuty(dutyData->currentValue);
			}

			debug(String("f")+timer1_pwm->getFrequency()+" d"+String(timer0_pwm->getDuty()),3);
			delay(100);
		} else {
			startupping = 0;
			free(freqData);
			free(dutyData);
			debug("ramp end ", 3);
		}

	}
	// else parse latestcommand
	else if (commandRead == 0) {
		latestMessage = parseCommand(latestCommand);
		commandRead = 1;
	}
}


String brushless::parseCommand(Command command){
	
  switch(command->type){

  case 'f':
	  timer1_pwm->setFrequency(command->value);
	  	  
	  return String("")+timer1_pwm->getFrequency();
  case 'n':
          timer1_pwm->setDuty(command->value);
	  return String(timer1_pwm->getDuty());

  case 'b':
	  timer0_pwm->setFrequency(command->value);
	  return String(timer0_pwm->getFrequency());
  case 'd':
	  timer0_pwm->setDuty(command->value);
	  return String(timer0_pwm->getDuty());
  case 'r':
          automa->setAutomaRate(command->value);
	  return String(automa->getAutomaRate());
  case 's':
	  startup();
	  return "ramp started";
  case 'p':
	return  String( "--QUERY--\n") +
		String("f_t1 ") + String(timer1_pwm->getFrequency())	+ String("\n") +
		String("d_t1 ") + String(timer1_pwm->getDuty())     	+ String("\n") +
		String("f_t0 ") + String(timer0_pwm->getFrequency()) 	+ String("\n") +
		String("d_t0 ") + String(timer0_pwm->getDuty())   	+ String("\n") +
		String("r")     + String(automa->getAutomaRate())	+ String("\n") +	
	        String( "----"); 
    

  default:
	return "";
  
  }
}

int brushless::setCommand(Command command) {
	latestCommand = command;
	commandRead = 0;
}
String brushless::getResponse() {
	return latestMessage;
}

int brushless::start(){
	debug("timer1 start",3);
	timer1_pwm->start();
}

