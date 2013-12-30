/*

 Il circuito prevede che i pin 2 3 e 4 siano collegati ai
 pin di comando dei buffer gate il cui input e' il segnale pwm generato dal timer.
 I 3 gate vanno connessi ai mosfet nella parte superiore.
 I pin 5,6 e 7 devono essere invece collegati direttamente ai mosfet della parte inferiore.

 */

#include "brushless.h"



#define NUM_STATES 6

byte states[NUM_STATES] = {
  B01000100,
  B10000100,
  B10001000,
  B00101000,
  B00110000,
  B01010000};

brushless::brushless(){

#ifndef F_CPU
communicator::logToSerial("F_CPU undefined", 0)
#endif

 communicator::logToSerial( String("Entering constructor for: ") + __func__, 6);

  DDRD       |= B11111100;  // set pin [2,7] as output
  PORTD       = states[0];  // set up first state on pins 2,6

  frequency   = 900;
  duty        =  90;
  refreshRate = 120;

  cpmCounter  = 0;
  stato       = 0;

  timer1_init();

}

int brushless::timer1_init(){

  pinMode(10,OUTPUT);
  pinMode(9,OUTPUT);
  /*
    Prescaler is configured like this:

   (1 << CS10): divide by 1, 64, 1024
   (1 << WGM13): 16 bit Phase+Frequency correct, TOP =ICR1
   (1 << COM1B1): non-inverting, and inverting?????
   */

  ICR1   = frequency;
  OCR1B  = map(duty,0,255,0,frequency);
  

  TCCR1B = (1 << CS10) | (1 << WGM13);
  TCCR1A = _BV(COM1B1); // SET ONE PWM CHANNEL TO NON-INVERTING DUTY CYCLE   
  
  

  TIMSK1 = _BV(OCIE1B);  //signal handler association
}


void brushless::startupcalc(startupData valueData, int slow)
 { 
   //start_values ritorno;
   int delta = valueData->currentValue - valueData->end;
   float minus = delta * valueData->decrement;
   if (minus >= 1)
   {
     valueData->currentValue = valueData->currentValue - floor(minus);
   }
   else
   {
     if (slow == 1)
     {
       valueData->resto = valueData->resto + minus;
       if (valueData->resto >=1)
       {
         valueData->currentValue = valueData->currentValue - floor(valueData->resto);
         valueData->resto = valueData->resto - floor(valueData->resto);
       }
 
     }
     else if ( slow == 0 )
     {
       valueData->currentValue = valueData->currentValue - 1;
     }
   }   
 }



 int brushless::startup(){ 

  char numstr[21]; // for holding temporary string
  communicator::logToSerial(String("Entering brushless::") + __func__ , 5);
 

startupData freqData = (startupData)malloc(sizeof(_startup_data));
freqData->start = 900;
freqData->end = 320;
freqData->decrement = 0.1;
freqData->currentValue = freqData->start;
freqData->resto = 0;

startupData dutyData = (startupData)malloc(sizeof(_startup_data));
dutyData->start = 90;
dutyData->end = 65;
dutyData->decrement = 0.2;
dutyData->currentValue = dutyData->start;
dutyData->resto = 0;

startupData refreshData = (startupData)malloc(sizeof(_startup_data));
refreshData->start = 120;
refreshData->end = 30;
refreshData->decrement = 0.1;
refreshData->currentValue = refreshData->start;
refreshData->resto = 0;

   
 
   while ((freqData->currentValue > freqData->end) || (dutyData->currentValue > dutyData->end)||(refreshData->currentValue > refreshData->end))
   {
 
     if (freqData->currentValue > freqData->end)
     {
       startupcalc(freqData, 1);
       setFrequency(freqData->currentValue);
     }
	delay(15); 
     if (dutyData->currentValue > dutyData->end )
     {
       startupcalc(dutyData,1);
       setDuty(dutyData->currentValue);
     }
	delay(15);
     if (refreshData->currentValue > refreshData->end )
     {
       startupcalc(refreshData,1);
       setRefreshRate(refreshData->currentValue);
     }

 

  String tempString = String(itoa(freqData->currentValue, numstr, 10 )) + "," +
                      itoa(dutyData->currentValue, numstr, 10 ) + "," +
                      itoa(refreshData->currentValue, numstr, 10 );
 
 communicator::logToSerial(tempString , 6 );



   delay(150);
   }
}

int brushless::getFrequency(){
  return frequency;
}
int brushless::getDuty(){
  return duty;
}
int brushless::getRefreshRate(){
  return refreshRate;
}

int brushless::setFrequency(int val){
  /*
   in questo punto sarebbe bello determinare un range di
   valori utili e mapparlo su una scala di valori semplici tipo 0 - 100
   per ora passiamo tutto
   */
  char numstr[21]; // for holding temporary string

  if(val == frequency){ //skip if the value is the same

  String tempString =String("setFrequency error: same value ") +
                      itoa(frequency, numstr, 10 );
 
   communicator::logToSerial(tempString , 0);
    return frequency; 
  }
  
  if (frequency > val){
  frequency = val; // Assign the value set to frequency
  setDuty(duty);   // to avoid duty out of range duty is decreased before the frequency 
  ICR1 = frequency;
  }
  
  if (frequency < val){
  frequency = val; // Assign the value set to frequency
  ICR1 = frequency;
  setDuty(duty);
  }

  return frequency;
}

int brushless::setDuty(int val){

  if(val < 0 || val >= 255) return -1;

  duty  = val;
  OCR1B = map(duty,0,255,0,frequency);
  //OCR1A = map(duty,0,255,0,frequency);
  return duty;

}

int brushless::setRefreshRate(int val){

  /*
  necessaria un analisi sperimentale di questo valore
   */
  char numstr[21]; // for holding temporary string

  if(val == refreshRate){ //skip if the value is the same
  String tempString =String("setRefreshRate error: same value ") +
                      itoa(refreshRate, numstr, 10 );
 
   communicator::logToSerial(tempString , 2);

    return refreshRate;
  }
   
  refreshRate = val; // set the new refresh rate
  


  return refreshRate;
}


int brushless::eventHandler(){

  cpmCounter++;

  if(cpmCounter >= refreshRate){

    // iterazione attraverso gli stati dell'automa
    stato      = ++stato % NUM_STATES;
    PORTD      = states[stato];

    cpmCounter = 0;
  }
}


