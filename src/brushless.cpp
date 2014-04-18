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
  
  commandRead = 0;
  
  timer0_pwm   = new timer0();
  timer1_pwm   = new timer1();

// TODO bring timer 2 here

  automa	     = new mosfetSequencecontroller();
  //automa->setAutomaRate(RAMP_INIT_REFREASHRATE);
  automa->init();

  latestCommand = (Command)malloc(sizeof(_command));
  latestCommand->type = 'n';

  timer0_pwm->start();
  

  

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
  int time=msTime;
  float ang= (float)gain * msTime * 0.001; 
  unsigned int freqHz = ang + ssGain;
  //debug (freqHz,3);
  //debug(time,3);
  return freqHz;
}
void brushless::startuppone() {
  // inizializzazione startup ( da spostare e migliorare ) ma deve esserci sempre la struttura cosi si può comandare il motore con delle rette invece che settare valori uno a uno
  freqData = (startupData) malloc(sizeof(_startup_data));
  freqData->start = timer1_pwm->getFrequency();  //start value in Hz
  freqData->end = RAMP_END_FREQUENCY_T1;   //end value in Hz
  freqData->gain = RAMP_GAIN_FREQUENCY_T1; //gain 
  freqData->currentValue = timer1_pwm->getFrequency();
  
  startup();
  
  freqData->end = 2000;   //end value in Hz
  freqData->gain = 400; //gain 
  startup();
  
    freqData->end = 2010;   //end value in Hz
  freqData->gain = 10; //gain 
  startup();
  
    freqData->end = 2200;   //end value in Hz
  freqData->gain = 100; //gain 
  startup();
  
      freqData->end = 2210;   //end value in Hz
  freqData->gain = 5; //gain 
  startup();
  
//      freqData->end = 4000;   //end value in Hz
//  freqData->gain = 10; //gain 
//  startup();
  
  free(freqData);
  free(dutyData);
}

void brushless::startup() {

  debug(String("starting ramp") , 3);

  //freqData = (startupData) malloc(sizeof(_startup_data));
  freqData->start = timer1_pwm->getFrequency();  //start value in Hz
  //freqData->end = 4000;   //end value in Hz
  //freqData->gain = 1000; //gain 
  freqData->currentValue = timer1_pwm->getFrequency();
  
  dutyData = (startupData) malloc(sizeof(_startup_data));
  dutyData->start = timer0_pwm->getDuty();
  dutyData->end = RAMP_FIN_DUTY_T0;
  dutyData->gain = 0.01;
  dutyData->currentValue = dutyData->start;
  //dutyData->resto = 0;

  startupping();
    
}

  //if startup mode do startup

  void brushless::startupping() {
  //Resetta il riferimento temporale
  msTime = 0;
  
    while ((freqData->currentValue < freqData->end) )//||  (dutyData->currentValue > dutyData->end)) 
    {

      if (freqData->currentValue < freqData->end) {

        //startupcalc(freqData, 1);
        freqData->currentValue = getStartupValueHz(freqData->gain, freqData->start);
        timer1_pwm->setFrequency(freqData->currentValue);
      }

//      if (dutyData->currentValue > dutyData->end) {
//        //startupcalc(dutyData, 1);
//        dutyData->currentValue = getStartupValueHz(dutyData->gain, dutyData->start);
//        timer0_pwm->setDuty(dutyData->currentValue);
//      }
  
      //debug(String("f:")+timer1_pwm->getFrequency()+String(" top:")+timer1_pwm->getTop()+" time:"+msTime+" d:"+String(timer0_pwm->getDuty()),3);
      //debug(String("f:")+timer1_pwm->getFrequency()+" time:"+msTime,3);
      //delay(100);
      
     
    } 
   

      debug(String("Startup took: ")+msTime+" ms", 3);
      
    }

  

void brushless::iterate() {

  // else parse latestcommand
  if (commandRead == 0) {
    latestMessage = parseCommand(latestCommand);
    commandRead = 1;
  }
}

void brushless::setTime(){
  msTime = msTime + 10;
}

String brushless::parseCommand(Command command){

  switch(command->type){

  case 't':
    return  String(msTime)+ "ms\n";    
    
  case 'f':
    timer1_pwm->setFrequency(command->value);
    return String(timer1_pwm->getFrequency());
    
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
    return angSpeed();
    
  case 'q':
    startuppone();
    return "Startup end";
    
    case 's':
    startup();
    return "Startup end";

  case 'u': 
    freqData->end = command->value;//io ho settato diretto Ale, non incazzarti se vuoi sotto ci sono delle SET :D
    return String(freqData->end); 
    
  case 'i':
    freqData->gain = (command->value); 
    return String(freqData->gain); 

  case 'o':
    return "GAIN:"+String(freqData->gain)+" END:"+String(freqData->end); 
    
  case 'p':
    return  String( "\n--QUERY--\n") +
      String("f_t1 ") + String(timer1_pwm->getFrequency())	+ String(" Hz\n") +
      String("TOP_t1 ") + String(timer1_pwm->getTop())	        + String("\n") +
      String("d_t1 ") + String(timer1_pwm->getDuty())     	+ String("\n") +
      String("f_t0 ") + String(timer0_pwm->getFrequency()) 	+ String("\n") +
      String("d_t0 ") + String(timer0_pwm->getDuty())   	+ String("\n") +
      String( "----\n"); 
      
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

void brushless::start(){
  debug("timer1 start",5);
  timer1_pwm->start();
    debug("timer1 started",3);
}

String brushless::angSpeed(){
        int conv_Hz_rpm = 60 / NUM_STATES; // questo passaggio serve per non causare un overflow nella riga dopo !
	unsigned int RPM_e = floor(timer1_pwm->getFrequency() * conv_Hz_rpm);
	unsigned int RPM_m = floor(RPM_e * 2 / NUM_POLES);
	int rads_e = floor(RPM_e /60 * 2 * 3.14159);
	int rads_m = floor(RPM_m /60 * 2 * 3.14159);
       
	return "RPM elettrici:"+String(RPM_e)+", RPM meccanici:"+String(RPM_m)+", RAD/s elettrici:"+String(rads_e)+", RAD/s meccanici:"+String(rads_m);
}


//PROBLEMSSSSS
//  int brushless::setStartupfreqEnd (int val) {
//      if (val < 0 || val > 30000)
//      return -1;
//      
//      freqData->end = val;   //end value in Hz
//  
//      return 0;
//}
//
//  int brushless::setStartupfreqGain (int val) {
//      if (val < 0 || val > 2500)
//      return -1;
//      
//      freqData->gain = val;   //end value in Hz
//  
//      return 0;
//}


