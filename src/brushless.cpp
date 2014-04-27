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

void brushless::motor_init() {
  
  
  // procedura di inizializzazione del motore	
  timer0_pwm->setDuty(1);
  msTime=0;
  
  // da creare define 
  int gains = 35;
  int dutys = 50;
  int gaine = 30;
  int dutye =80;
  automa->stop();
  automa->setState(0);
  debug(__func__+String(" start duty ramp"),3);
  
  while ( timer0_pwm->getDuty() <= dutys)
  {
  	timer0_pwm->setDuty(gains * msTime * 0.001);
  }
  
  debug(__func__+String(" first ramp took")+msTime,3);
  timer1_pwm->setFrequency(DEFAULT_T1_INIT_FREQUENCY);
  automa->start();
  msTime=0;
  
  while ( timer0_pwm->getDuty() <= dutye)
  {
  	timer0_pwm->setDuty(gaine * msTime * 0.001 + dutys);
  }
  debug(__func__+String(" second ramp took")+msTime,3);
  
  freqData = (startupData) malloc(sizeof(_startup_data));
  freqData->start = timer1_pwm->getFrequency();  //start value in Hz
  freqData->end = RAMP_END_FREQUENCY_T1;   //end value in Hz
  freqData->gain = RAMP_GAIN_FREQUENCY_T1; //gain 
  //freqData->currentValue = timer1_pwm->getFrequency();

  startup(); 
  
  free(freqData);
  free(dutyData);
}

void brushless::startup() {

  debug(String("starting ramp from:")+ String(timer1_pwm->getFrequency())+ " to:" + String(freqData->end)+ " at gain:"+String(freqData->gain), 3);

  freqData->start = timer1_pwm->getFrequency();  //start value in Hz
 
 
  //get motor ready to accelerate
  timer0_pwm->setDuty(99);
  //Resetta il riferimento temporale
  msTime = 0;
  


  while ((timer1_pwm->getFrequency() < freqData->end) )
  {

    if (timer1_pwm->getFrequency() < freqData->end) {

      timer1_pwm->setFrequency(getStartupValueHz(freqData->gain, freqData->start));
    }
	
    //debug(String("f:")+timer1_pwm->getFrequency()+String(" top:")+timer1_pwm->getTop()+" time:"+msTime+" d:"+String(timer0_pwm->getDuty()),3);
    //debug(String("f:")+timer1_pwm->getFrequency()+" time:"+msTime,3);
    //delay(100);
    
  } 
  
  //reduce duty for steady speed 
 // timer0_pwm->setDuty(90);

  debug(String("Startup took: ")+msTime+" ms", 3);
    
}

  

void brushless::iterate() {

  // parse latestcommand
  if (commandRead == 0) {
    latestMessage = parseCommand(latestCommand);
    commandRead = 1;
  }
}

void brushless::incrementTime(){
  msTime = msTime + 10;
}

String brushless::parseCommand(Command command){

  switch(command->type){

// Print time
  case 't':
    return  String(msTime)+ "ms\n";    
    
// Set frequency commutation of state machine
  case 'f':
    timer1_pwm->setFrequency(command->value);
    return String(timer1_pwm->getFrequency());
    
// Set duty cycle of pwm
  case 'n':
    timer1_pwm->setDuty(command->value);
    return String(timer1_pwm->getDuty());

// Set frequency pwm switching
  case 'b':
    timer0_pwm->setFrequency(command->value);
    return String(timer0_pwm->getFrequency());

// Set duty
  case 'd':
    timer0_pwm->setDuty(command->value);
    return String(timer0_pwm->getDuty());

// Print frequency values
  case 'r':
    return angSpeed();
    

// Motor init. procedure    
  case 's':
    motor_init();
    return "Startup end";
    
  case 'z':
    automa->start();
    return "Commuting";

// Set end value of startup ramp
  case 'u': 
    freqData->end = command->value;
    return String(freqData->end); 
    
// Set gain for startup ramp
  case 'i':
    freqData->gain = (command->value); 
    return String(freqData->gain); 

// Debug print
  case 'o':
    return "GAIN:"+String(freqData->gain)+" END:"+String(freqData->end); 
    
// Formatted print for parsing
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
	unsigned int RPM_e = floor( (timer1_pwm->getFrequency()/NUM_STATES) * 60);
	unsigned int RPM_m = floor(RPM_e /(NUM_POLES/2));
	int rads_e = floor(RPM_e /60 * 2 * 3.14159);
	int rads_m = floor(RPM_m /60 * 2 * 3.14159);
       
	return "RPM elettrici:"+String(RPM_e)+", RPM meccanici:"+String(RPM_m)+", RAD/s elettrici:"+String(rads_e)+", RAD/s meccanici:"+String(rads_m);
}

  int brushless::setStartupfreqEnd (int val) {
      if (val < 0 || val > 30000)
      return -1;
      
      freqData->end = val;   //end value in Hz
  
      return 0;
}

  int brushless::setStartupfreqGain (int val) {
      if (val < 0 || val > 2500)
      return -1;
      
      freqData->gain = val;   //end value in Hz
  
      return 0;
}

ISR(TIMER1_COMPA_vect) {
	automa->commutePole();
}


