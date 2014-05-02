#include "atmegax8.h"
#include "brushless.h"
#include "timer1.cpp"
#include "timer0.cpp"
#include "timer.h"


timer1 *automa_frequency = NULL;
timer0 *pwm = NULL;
mosfetSequencecontroller * automa = NULL;


brushless::brushless() {
  debug(String("Entering constructor for: ") + __func__,3);
  rampPWMDuty.gain = 5;
  rampPWMDuty.offset = 1;
  rampPWMDuty.currentValue = 0;
  rampPWMDuty.end = 95;
  rampAutomaFrequencyA.gain = 100;
  rampAutomaFrequencyA.offset = DEFAULT_T1_INIT_FREQUENCY;
  rampAutomaFrequencyA.currentValue = 0;
  rampAutomaFrequencyA.end = 1000;
  rampAutomaFrequencyB.gain = 100;
  rampAutomaFrequencyB.offset = DEFAULT_T1_INIT_FREQUENCY;
  rampAutomaFrequencyB.currentValue = 0;
  rampAutomaFrequencyB.end = 5000;
  
  // initialize timer objects
  pwm   = new timer0();
  automa_frequency   = new timer1();
  // TODO bring timer 2 here

  // Get state machine ready for callbacks
  automa	     = new mosfetSequencecontroller();
  automa->init();

  // allocate buffer for char array
  latestCommand = (Command)malloc(sizeof(_command));
  latestCommand->type = 'n';
}

int brushless::getStartupOpenLoopValue(ramp ramp) {

  // proportional open loop controller
  // y = K * t + y0 [Hz]

  float ang = (float)ramp.gain * msTime * 0.001;
  int OpenLoopValue = ang + ramp.offset; 

  //debug(String("msTime ")+msTime+" gain "+ ramp.gain + "offset " + ramp.offset + "value" + OpenLoopValue,3);
  //debug (OpenLoopValue,3);
  return OpenLoopValue; 

}


int brushless::setStartupState(int state){

  switch(state){

  // start pwm signal
  case startupStateMotorOff:
    pwm->start();
	automa_frequency->start();
    //TODO tirare giu tutti i pin logici setstate(OFF)
    startupState = startupStatePWMStarted;
    return  0;
   
   // Stop motor for aligning rotor
   case startupStatePWMStarted:
    automa->stop();
    automa->setState(0);
    //debug(String("In function: ") + __func__,3);
    debug(String("PWM Started - Commencing rotor alignment ") ,3);
    startupState = startupStateRotorAligned;
    return  0;    
    
   // start increasing pwm duty without changing automa state
   case startupStateRotorAligned:
  	pwm->setDuty(getStartupOpenLoopValue(rampPWMDuty));
	// keep rotor fixed, until pwm is 50% of end duty
	if ( pwm->getDuty() >= rampPWMDuty.end/2 ){		
		startupState = startupStateSetupAutomaRampA;
 	 }
	return 0;
    
   // start automa
   case startupStateSetupAutomaRampA:

    // start drive sequence
	//automa_frequency->start();
    automa->start();
    // set ramp duty offset and reset clock
    rampPWMDuty.offset = pwm->getDuty();
    msTime=0;

    //debug(String("In function: ") + __func__,3);
    debug(String("Starting Automa Ramp A ") ,3);
    startupState = startupStateAutomaRampA;
    return  0;    

   // increase frequency of automa and pwm duty until max duty value is reached
   case startupStateAutomaRampA:
	// raise duty until end duty
      if (  pwm->getDuty() < rampPWMDuty.end )
      pwm->setDuty(getStartupOpenLoopValue(rampPWMDuty));

	// raise automa frequency until end frequency
      if (  automa_frequency->getFrequency() < rampAutomaFrequencyA.end )
      automa_frequency->setFrequency(getStartupOpenLoopValue(rampAutomaFrequencyA));

	// set next state once pwm and duty reach end value
      if (  pwm->getDuty() >= rampPWMDuty.end  &&  automa_frequency->getFrequency() >= rampAutomaFrequencyA.end  )
      {
          startupState = startupStateSetupAutomaRampB;
      }
	return  0;
   
   // increase automa frequency until max automa frequency of ramp A
   case startupStateSetupAutomaRampB:

    // set pwm offset and reset clock
    rampAutomaFrequencyB.offset = pwm->getFrequency();
    msTime=0;

    //debug(String("In function: ") + __func__,3);
    debug(String("Starting Automa Ramp B ") ,3);
    startupState = startupStateAutomaRampB;
    return  0;
   
   // continue increasing automa frequency until max automa frequency of ramp B
   case startupStateAutomaRampB:
      automa_frequency->setFrequency(getStartupOpenLoopValue(rampAutomaFrequencyB));
      if ( automa_frequency->getFrequency() >= rampAutomaFrequencyB.end)
      {
          startupState = startupStateStartupFinished;
      }
     return  0;
    
    // finish
   case startupStateStartupFinished:
	// reduce duty for steady speed 
	// pwm->setDuty(90);
	//debug(String("In function: ") + __func__,3);
	debug(String("Startup Finished. Time is: ")+msTime+" ms", 3);
     	return  1;


  default:
    return -1;

  }
}

int brushless::startupCallback() {

//debug(String(startupState),3);

if(setStartupState(startupState) == 1)
    	starting = 0;
    	
    	return 0;
}

  

int brushless::iterate() {

  // parse latestcommand
  if (commandRead == 0) {
    latestMessage = parseCommand(latestCommand);
    commandRead = 1;
  }
  
   if (starting == 1) {
   startupCallback();
   }
   else{
   
	// future cltf goes here
   }
      	return 0;
}

int brushless::incrementTime(){
  msTime = msTime + 10;
      	return 0;
}

String brushless::parseCommand(Command command){

  switch(command->type){

// Print time
  case 't':
    return  String(msTime)+ "ms\n";    
    
// Set pwm frequency
  case 'f':
    pwm->setFrequency(command->value);
    return String(pwm->getFrequency());
    
// Set pwm duty cycle
  case 'd':
    pwm->setDuty(command->value);
    return String(pwm->getDuty());

// Set automa frequency
  case 'a':
    automa_frequency->setFrequency(command->value);
    return String(automa_frequency->getFrequency());

// Print frequency values
  case 'r':
    return angSpeed();
    
// Start motor    
  case 's':
    starting=1;
startupState = startupStateMotorOff;
    return "Starting";
    
// Set end value of startup ramp
  case 'u': 
    rampAutomaFrequencyA.end = command->value;
    return String(rampAutomaFrequencyA.end); 
    
// Set gain for startup ramp
  case 'i':
    rampAutomaFrequencyA.gain = (command->value); 
    return String(rampAutomaFrequencyA.gain); 

// Debug print
  case 'o':
    return "GAIN:"+String(rampAutomaFrequencyA.gain)+" END:"+String(rampAutomaFrequencyA.end); 
    
// Formatted print for parsing
  case 'p':
    return  String( "--QUERY--\n") +
      String("f_t1 ") + String(automa_frequency->getFrequency())	+ String(" Hz\n") +
      String("TOP_t1 ") + String(automa_frequency->getTop())	        + String("\n") +
      String("d_t1 ") + String(pwm->getDuty())     	+ String("\n") +
      String("f_t0 ") + String(pwm->getFrequency()) 	+ String("\n") +
      String("d_t0 ") + String(pwm->getDuty())   	+ String("\n") +
      String( "----\n"); 
      
  default:
    return "";

  }
}

int brushless::setCommand(Command command) {
  latestCommand = command;
  commandRead = 0;
      	return 0;
}

String brushless::getResponse() {
  return latestMessage;
}

int brushless::start(){
// NOT USED
    	return 0;
}

String brushless::angSpeed(){
	unsigned int RPM_e = floor( (automa_frequency->getFrequency()/NUM_STATES) * 60);
	unsigned int RPM_m = floor(RPM_e /(NUM_POLES/2));
	int rads_e = floor(RPM_e /60 * 2 * M_PI);
	int rads_m = floor(RPM_m /60 * 2 * M_PI);
       
	return "RPM elettrici:"+String(RPM_e)+", RPM meccanici:"+String(RPM_m)+", RAD/s elettrici:"+String(rads_e)+", RAD/s meccanici:"+String(rads_m);
}

  int brushless::setStartupFreqEnd (int val) {
      if (val < 0 || val > 30000)
      return -1;
      
      rampAutomaFrequencyA.end = val;   //end value in Hz
      return 0;
}

  int brushless::setStartupFreqGain (int val) {
      if (val < 0 || val > 2500)
      return -1;
      
      rampAutomaFrequencyA.gain = val;   //end value in Hz
      return 0;
}


brushless::~brushless() {
 	free(pwm); 
 	free(automa); 
 	free(automa_frequency ); 
 }

ISR(TIMER1_COMPA_vect) {
	automa->commutePole();
}


