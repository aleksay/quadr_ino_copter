
#include "atmegax8.h"
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

  msTime = 0;  
  startupping = 0;
  commandRead = 0;

  timer1_pwm   = new timer1();
  timer0_pwm   = new timer0();
// TODO bring timer 2 here

  automa	     = new mosfetSequencecontroller();
  //automa->setAutomaRate(RAMP_INIT_REFREASHRATE);
  automa->init();

  latestCommand = (Command)malloc(sizeof(_command));
  latestCommand->type = 'n';

  timer0_pwm->start(0);
}

//void brushless::startupcalc(startupData valueData, int slow) {
//	//start_values ritorno;
//	unsigned int delta = valueData->currentValue - valueData->end;
//	
//	float minus = delta * valueData->decrement;
//	if (minus >= 1) {
//		valueData->currentValue = valueData->currentValue - floor(minus);
//	} else {
//		if (slow == 1) {
//			valueData->resto = valueData->resto + minus;
//			if (valueData->resto >= 1) {
//				valueData->currentValue = valueData->currentValue	- floor(valueData->resto);
//				valueData->resto = valueData->resto - floor(valueData->resto);
//			}
//		} else if (slow == 0) {
//			valueData->currentValue = valueData->currentValue - 1;
//		}
//	}
//}

int brushless::getStartupValueHz(int gain, int ssGain) {

  // proportional open loop controller
  // y = K * t + Kstart [Hz]
  int freqHz = gain * 0.001 * msTime + ssGain;
  return freqHz;
  
}


int brushless::startup() {

  debug(String("starting ramp") , 5);

  freqData = (startupData) malloc(sizeof(_startup_data));
  freqData->start = 244;  //start value in Hz
  freqData->end = 4000;   //end value in Hz
  freqData->gain = 1252; //gain
  
  dutyData = (startupData) malloc(sizeof(_startup_data));
  dutyData->start = timer0_pwm->getDuty();
  dutyData->end = RAMP_FIN_DUTY_T0;
  dutyData->gain = 0.01;
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

        //startupcalc(freqData, 1);
        freqData->currentValue = getStartupValueHz(freqData->gain, freqData->start);
        timer1_pwm->setFrequency(freqData->currentValue);
      }

      if (dutyData->currentValue > dutyData->end) {
        //startupcalc(dutyData, 1);
        dutyData->currentValue = getStartupValueHz(dutyData->gain, dutyData->start);
        timer0_pwm->setDuty(dutyData->currentValue);
      }

      debug(String("f")+timer1_pwm->getFrequency()+" d"+String(timer0_pwm->getDuty()),3);
      delay(100);
    } 
    else {
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

//void brushless::printTime(){
//  String debString=String("msTime is") + msTime + String("ms");
//  debug(debString,3);
//}

void brushless::setTime(){
  msTime = msTime + 50;
}

String brushless::parseCommand(Command command){

  switch(command->type){

  case 't':
    return  String("")+msTime;    
    
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
	  "valore TOP timer 1 "+OCR1A+ String("\n") +
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
  debug("timer1 start",5);
  timer1_pwm->start();
    debug("timer1 started",3);
}


