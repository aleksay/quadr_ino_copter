#include "atmegax8.h"
#include "brushless.h"
#include "timer1.cpp"
#include "timer0.cpp"
#include "timer.h"


timer1 *automa_frequency = NULL;
timer0 *pwm = NULL;
mosfetSequencecontroller * automa = NULL;


brushless::brushless() {
  debug("Entering constructor");
  rampPWMDuty.gain = 15;
  rampPWMDuty.offset = 1;
  rampPWMDuty.currentValue = 0;
  rampPWMDuty.end = 95;
  rampAutomaFrequencyA.gain = 50;
  rampAutomaFrequencyA.offset = DEFAULT_T1_INIT_FREQUENCY;
  rampAutomaFrequencyA.currentValue = 0;
  rampAutomaFrequencyA.end = 350;
  rampAutomaFrequencyB.gain = 50;
  rampAutomaFrequencyB.offset = DEFAULT_T1_INIT_FREQUENCY;
  rampAutomaFrequencyB.currentValue = 0;
  rampAutomaFrequencyB.end = 1000;
  
  // initialize timer objects
  pwm   = new timer0();
  automa_frequency   = new timer1();
  // TODO bring timer 2 here

  // Get state machine ready for callbacks
  automa	     = new mosfetSequencecontroller();
  automa->init();
  
  setStartupState(startupState_MotorOff);
  
  // allocate buffer for char array
  latestCommand = (Command)malloc(sizeof(_command));
  latestCommand->type = 'n';
}

int brushless::getStartupOpenLoopValue(ramp ramp) {

  // proportional open loop controller
  // y = K * t + y0 [Hz]

  float ang = (float)ramp.gain * msTime * 0.001;
  int OpenLoopValue = ang + ramp.offset; 

  debug("msTime: %d, gain: %d, offset: %d, value: %d",msTime,ramp.gain,ramp.offset,OpenLoopValue);
  //debug ("%d",OpenLoopValue);
  return OpenLoopValue; 

}


int brushless::setStartupState(int state){

  switch(state){

  // start pwm signal
  case startupState_MotorOff:
    automa->setOpenInverter(); //micro con tutti i pin logici low 
	
    startupState = startupState_MotorInit;
    return  0;
	
	case startupState_MotorInit:
    pwm->start();
	automa_frequency->start();
    //TODO tirare giu tutti i pin logici setstate(OFF)
    startupState = startupState_PWMStarted;
    return  0;
   
   // Stop motor for aligning rotor
   case startupState_PWMStarted:
    automa->stop();
    automa->setState(DEFAULT_INITIAL_STATE);
    debug("PWM Started - Commencing rotor alignment ");
    startupState = startupState_RotorAligned;
    return  0;    
    
   // start increasing pwm duty without changing automa state
   case startupState_RotorAligned:
  	pwm->setDuty(getStartupOpenLoopValue(rampPWMDuty));
	// keep rotor fixed, until pwm is 50% of end duty
	if ( pwm->getDuty() >= rampPWMDuty.end/2 ){		
		startupState = startupState_SetupAutomaRampA;
 	 }
	return 0;
    
   // start automa
   case startupState_SetupAutomaRampA:

    // start drive sequence
	//automa_frequency->start();
    automa->start();
    // set ramp duty offset and reset clock
    rampPWMDuty.offset = pwm->getDuty();
    msTime=0;


    debug("Starting Automa Ramp A");
    startupState = startupState_AutomaRampA;
    return  0;    

   // increase frequency of automa and pwm duty until max duty value is reached
   case startupState_AutomaRampA:
	// raise duty until end duty
      if (  pwm->getDuty() < rampPWMDuty.end )
      pwm->setDuty(getStartupOpenLoopValue(rampPWMDuty));

	// raise automa frequency until end frequency
      if (  automa_frequency->getFrequency() < rampAutomaFrequencyA.end )
      automa_frequency->setFrequency(getStartupOpenLoopValue(rampAutomaFrequencyA));

	// set next state once pwm and duty reach end value
      if (  pwm->getDuty() >= rampPWMDuty.end  &&  automa_frequency->getFrequency() >= rampAutomaFrequencyA.end  )
      {
          startupState = startupState_SetupAutomaRampB;
      }
	return  0;
   
   // increase automa frequency until max automa frequency of ramp A
   case startupState_SetupAutomaRampB:
  
    // set pwm offset and reset clock
    rampAutomaFrequencyB.offset = automa_frequency->getFrequency();
    msTime=0;   
  
    TCCR1B &= (0 << CS12) | ~(1 << CS11)  | (0 << CS10);
    automa_frequency->setPrescaler(1);
    automa_frequency->setFrequency(automa_frequency->getFrequency()+1);
    
    debug("Starting Automa Ramp B ");
    startupState = startupState_AutomaRampB;
    return  0;
   
   // continue increasing automa frequency until max automa frequency of ramp B
   case startupState_AutomaRampB:
      automa_frequency->setFrequency(getStartupOpenLoopValue(rampAutomaFrequencyB));
      if ( automa_frequency->getFrequency() >= rampAutomaFrequencyB.end)
      {   
	  
		
        startupState = startupState_StartupFinished;
      }
     return  0;
    
    // finish
   case startupState_StartupFinished:
	// reduce duty for steady speed 
	// pwm->setDuty(90);
	debug("Startup Finished. Time is[ms]: %d",msTime);
     	return  1;


  default:
    return -1;

  }
}

int brushless::startupCallback() {
debug("ciao");
 // debug("Startup state is: %d",startupState);

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
	
  case 'l':
    automa_frequency->setPrescaler(command->value);
    return String(automa_frequency->getPrescaler());

// Print frequency values
  case 'r':
    return angSpeed();
    
// Start motor    
  case 's':
    starting=1;
    //startupState = startupState_MotorOff;
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
      String("TOP_t1 ") + String(automa_frequency->getTop())	        + String("\n");    

      
  default:
    log_err("Invalid option");
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


