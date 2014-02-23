/*

 Il circuito prevede che i pin 2 3 e 4 siano collegati ai
 pin di comando dei buffer gate il cui input e' il segnale pwm generato dal timer.
 I 3 gate vanno connessi ai mosfet nella parte superiore.
 I pin 5,6 e 7 devono essere invece collegati direttamente ai mosfet della parte inferiore.

 */

#include "brushless.h"
#include "timer1.cpp"
#include "timer0.cpp"



#define NUM_STATES 6

#define RAMP_INIT_FREQUENCY 10000
#define RAMP_INIT_DUTY 1
#define RAMP_INIT_REFREASHRATE 1

#define RAMP_FIN_FREQUENCY 600
#define RAMP_FIN_DUTY 50
#define RAMP_FIN_REFREASHRATE 1

timer1 *timer1_pwm = NULL;
timer0 *timer0_pwm = NULL;
mosfetSequencecontroller * automa = NULL;

startupData freqData;
startupData dutyData;
startupData refreshData;



brushless::brushless() {

	timer1_pwm   = new timer1();
	timer1_pwm->setFrequency(RAMP_INIT_FREQUENCY);
	timer1_pwm->setDuty(RAMP_INIT_DUTY);
	
	timer0_pwm   = new timer0();
	timer0_pwm->setFrequency(T0_FREQUENCY);
	timer0_pwm->setDuty(T0_DUTY);
	//OCR0B

	automa	     = new mosfetSequencecontroller();
	automa->setAutomaRate(RAMP_INIT_REFREASHRATE);
	automa->init();

	latestCommand = (Command)malloc(sizeof(_command));
	latestCommand->type = 'n';

	timer0_pwm->start(0);

}
//volatile int cpmCounter = 0;
//ISR(TIMER1_COMPA_vect) {
// cpmCounter++;

//  if(cpmCounter >= automa->getAutomaRate()){

//    // iterazione attraverso gli stati dell'automa
//    automa->commutePole();
//    cpmCounter = 0;
//  }
//}
//ISR(TIMER1_OVF_vect){
////	timer1_pwm->_timer1_ovf_handler();
//}

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

	debug(String("Entering brushless::") + __func__, 5);

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
		||  (dutyData->currentValue > dutyData->end)
		||  (refreshData->currentValue > refreshData->end)) {

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
			debug("f"+String(timer1_pwm->getFrequency())+" d"+String(timer1_pwm->getDuty())+" r"+String(automa->getAutomaRate()),3);
			delay(100);
		} else {
			startupping = 0;
			free(freqData);
			free(dutyData);
			free(refreshData);
		}

	}
	// else parse latestcommand
	else if (commandRead == 0) {
			latestMessage = parseCommand(latestCommand);
			commandRead = 1;
	}
}


String brushless::parseCommand(Command command){
	
  //int r = -10;

  switch(command->type){

  case 'f':
	  
	  timer1_pwm->setFrequency(command->value);
	  return String(timer1_pwm->getFrequency());
  case 'd':
    timer1_pwm->setDuty(command->value);
	
	  return String(timer1_pwm->getDuty());

  case 'b':
	  timer0_pwm->setFrequency(command->value);
	  return String(timer0_pwm->getFrequency());
  case 'n':
	  timer0_pwm->setDuty(command->value);
	  return String(timer0_pwm->getDuty());
  case 'r':
    automa->setAutomaRate(command->value);
	  return String(automa->getAutomaRate());
  case 'p':
//		communicator::logToSerial(String( "--QUERY--\n") +
//		 			 String("f") + String(timer1_pwm->getFrequency())   + String("\n") +
//           String("d") + String(timer1_pwm->getDuty())        + String("\n") +
//           String("r") + String(automa->getAutomaRate())			+ String("\n") +
//		       String( "----"),3);
		return  String( "--QUERY--\n") +
				String("f_t1 ") + String(timer1_pwm->getFrequency())   	+ String("\n") +
				String("d_t1 ") + String(timer1_pwm->getDuty())     	+ String("\n") +
				String("f_t0 ") + String(timer0_pwm->getFrequency())   	+ String("\n") +
				String("d_t0 ") + String(timer0_pwm->getDuty())     	+ String("\n") +
				String("r") + String(automa->getAutomaRate())		+ String("\n") +
				String("b") + String(automa->getbit())			+ String("\n") +
		        String( "----"); 
    

  default:
				commandRead = 1;
				return String( "--D--");
    
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
timer1_pwm->start(0);

}

