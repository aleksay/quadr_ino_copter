#include "atmegax8.h"
#include "brushless.h"
#include "timer1.cpp"
#include "timer0.cpp"
#include "timer.h"


timer1 automa_frequency;
timer0 pwm;
mosfetSequencecontroller automa;


brushless::brushless() {
  debug("Entering constructor");
  rampPWMDuty.gain = 20;
  rampPWMDuty.offset = 1;
  rampPWMDuty.currentValue = 0;
  rampPWMDuty.end = 98;
  rampAutomaFrequencyA.gain = 50;
  rampAutomaFrequencyA.offset = DEFAULT_T1_INIT_FREQUENCY;
  rampAutomaFrequencyA.currentValue = 0;
  rampAutomaFrequencyA.end = 250;
  rampAutomaFrequencyB.gain = 150;
  rampAutomaFrequencyB.offset = DEFAULT_T1_INIT_FREQUENCY;
  rampAutomaFrequencyB.currentValue = 0;
  rampAutomaFrequencyB.end = 2800;
  

  // TODO bring timer 2 here

  // Get state machine ready for callbacks
  automa.init();
  
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
      automa.setOpenInverter(); //micro con tutti i pin logici low 
	  automa.stop();
  	  pwm.setDuty(DEFAULT_T0_INIT_DUTY);
      startupState = startupState_MotorInit;
      return  0;
	
  case startupState_MotorInit:
        pwm.start();
        automa_frequency.start();
        //TODO tirare giu tutti i pin logici setstate(OFF)
		
        startupState = startupState_PWMStarted;
        return  0;
	
   
   // Stop motor for aligning rotor
  case startupState_PWMStarted:
    
    automa.setState(DEFAULT_INITIAL_STATE);
    debug("PWM Started - Commencing rotor alignment");
	msTime=0; 
    startupState = startupState_RotorAligned;
    return  0;    
    
   // start increasing pwm duty without changing automa state
  case startupState_RotorAligned:

  	pwm.setDuty(getStartupOpenLoopValue(rampPWMDuty));
	// keep rotor fixed, until pwm is 50% of end duty

	if ( pwm.getDuty() >= 60 ){	

		startupState = startupState_SetupAutomaRampA;
 	 }
	return 0;
    
   // start automa
   case startupState_SetupAutomaRampA:

    // start drive sequence
	//automa_frequency.start();
    automa.start();
    // set ramp duty offset and reset clock
    rampPWMDuty.offset = pwm.getDuty();
    msTime=0;


    debug("Starting Automa Ramp A");
    startupState = startupState_AutomaRampA;
    return  0;    

   // increase frequency of automa and pwm duty until max duty value is reached
   case startupState_AutomaRampA:
	// raise duty until end duty
      if (  pwm.getDuty() < rampPWMDuty.end )
      pwm.setDuty(getStartupOpenLoopValue(rampPWMDuty));

	// raise automa frequency until end frequency
      if (  automa_frequency.getFrequency() < rampAutomaFrequencyA.end )
      automa_frequency.setFrequency(getStartupOpenLoopValue(rampAutomaFrequencyA));

	// set next state once pwm and duty reach end value
      if (  pwm.getDuty() >= rampPWMDuty.end  &&  automa_frequency.getFrequency() >= rampAutomaFrequencyA.end  )
      {
          startupState = startupState_SetupAutomaRampB;
      }
	return  0;
   
   
   /////////////////////////////////////////////////////////////////////////   
   /////////////////////////////////////////////////////////////////////////
   
   
   // increase automa frequency until max automa frequency of ramp A
   case startupState_SetupAutomaRampB:
  
    // set pwm offset and reset clock
    rampAutomaFrequencyB.offset = automa_frequency.getFrequency();
    msTime=0;   
  
    TCCR1B &= (0 << CS12) | ~(1 << CS11)  | (0 << CS10);

    automa_frequency.setPrescaler(1);
    automa_frequency.setFrequency(automa_frequency.getFrequency()+1);
    
    debug("Starting Automa Ramp B");
    startupState = startupState_AutomaRampB;
    return  0;

   /////////////////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////////// 
   
   // continue increasing automa frequency until max automa frequency of ramp B
   case startupState_AutomaRampB:
      automa_frequency.setFrequency(getStartupOpenLoopValue(rampAutomaFrequencyB));
      if ( automa_frequency.getFrequency() >= rampAutomaFrequencyB.end)
      {   
	  
		
        startupState = startupState_StartupFinished;
      }
     return  0;
    
    // finish
   case startupState_StartupFinished:
	// reduce duty for steady speed 
	// pwm.setDuty(90);
	debug("Startup Finished. Time is[ms]: %d",msTime);
     	return  1;


  default:
	log_err("invalid state:%d",state);
    return -1;

  }
}

int brushless::startupCallback() {

//debug("Startup state is: %d",startupState);

if(setStartupState(startupState) == 1)
    	starting = 0;
    	
    	return 0;
}

  

int brushless::iterate() {

  // parse latestcommand
  if (commandRead == 0) {
    parseCommand(latestCommand);
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

int brushless::parseCommand(Command command){

  //debug("Received command.type:%c",command.type);
  switch(command->type){

// Print time
  case 't':
    free(command); 
    log_info("%dms",msTime);    
    return  0;
    
// Set pwm frequency
  case 'f':
    pwm.setFrequency(command->value);
    free(command); 
    log_info("pwm.getFrequency():%d",pwm.getFrequency());
    return 0;
    
// Set pwm duty cycle
  case 'd':
    pwm.setDuty(command->value);
    free(command); 
    log_info("pwm.getDuty():%d",pwm.getDuty());
    return 0;

// Set automa frequency
  case 'a':
    automa_frequency.setFrequency(command->value);
    free(command); 
    log_info("automa_frequency.getFrequency():%d",automa_frequency.getFrequency());
    return 0;
// Set prescaler value
  case 'l':
    automa_frequency.setPrescaler(command->value);
    free(command);
    log_info("automa_frequency.getPrescaler function is missing!");
    return 0;

// Print angular speed
  case 'b':
    free(command);
    //log_info("angSpeed():%d",angSpeed()); // BROKEN
    return 0;

// Print free RAM
  case 'r':
     free(command); 
     log_info("freeRAM = %d", freeRam());
     return 0;    

// watchdog reset
  case 'R':
     free(command);
     log_info("resetting...");
     wdt_sw_reset(); // broken for now ??
     return 0;
	
// Start motor    
  case 's':
     free(command); 
    starting=1;
    //startupState = startupState_MotorOff;
    log_info("Starting...");
    return 0;
    
// Set end value of startup ramp
  case 'u': 
    rampAutomaFrequencyA.end = command->value;
    free(command);
    log_info("rampAutomaFrequencyA.end = %d", rampAutomaFrequencyA.end);
    return 0; 
    
// Set gain for startup ramp
  case 'i':
    rampAutomaFrequencyA.gain = (command->value);
    free(command);
    log_info("rampAutomaFrequencyA.gain = %d", rampAutomaFrequencyA.gain);
    return 0; 

// Debug print
  case 'o':
    free(command);
    log_info("GAIN:%d,  END:%d", rampAutomaFrequencyA.gain, rampAutomaFrequencyA.end);
    return 0; 
    
// Formatted print for parsing
  case 'p':
    free(command);  
    log_info("--QUERY--\nf_t1 %d Hz\n TOP_t1 :%u", automa_frequency.getFrequency(), automa_frequency.getTop() );
    return  0;
          
  default:
     free(command); 
     log_warn("empty message");
     return 0;

  }
}

int brushless::setCommand(Command command) {
  latestCommand = command;
  commandRead = 0;
      	return 0;
}

int brushless::angSpeed(){
	unsigned int RPM_e = floor( (automa_frequency.getFrequency()/NUM_STATES) * 60);
	unsigned int RPM_m = floor(RPM_e /(NUM_POLES/2));
	int rads_e = floor(RPM_e /60 * 2 * M_PI);
	int rads_m = floor(RPM_m /60 * 2 * M_PI);
       log_info("RPM elettrici:%u, RPM meccanici:%u, RAD/s elettrici:%d, RAD/s meccanici:%d",RPM_e, RPM_m, rads_e, rads_m);
        return 0;
}

  int brushless::setStartupFreqEnd (int val) {
      if (val < 0 || val > 30000)
	log_err("invalid start freq:%d",val);
      return -1;
      
      rampAutomaFrequencyA.end = val;   //end value in Hz
      return 0;
}

  int brushless::setStartupFreqGain (int val) {
      if (val < 0 || val > 2500)
	log_err("invalid start gain:%d",val);
      return -1;
      
      rampAutomaFrequencyA.gain = val;   //end value in Hz
      return 0;
}


brushless::~brushless() {
 }

ISR(TIMER1_COMPA_vect) {
	automa.commutePole();
}


