
/*

 Il circuito prevede che i pin 2 3 e 4 siano collegati ai
 pin di comando dei buffer gate il cui input e' il segnale pwm generato dal timer.
 I 3 gate vanno connessi ai mosfet nella parte superiore.
 I pin 5,6 e 7 devono essere invece collegati direttamente ai mosfet della parte inferiore.


 */
#include <Arduino.h>
#include "brushless.h"

#ifndef F_CPU
#define F_CPU 8000000.0
#endif

#define NUM_STATES 6
//#define DEBUG

byte states[NUM_STATES] = {
  B01000100,
  B10000100,
  B10001000,
  B00101000,
  B00110000,
  B01010000};

brushless::brushless(){

//  Serial.print("Entering constructor for: ");
//  Serial.println(__FUNCTION__);

  DDRD       |= B11111100;  // set pin [2,7] as output
  PORTD       = states[0];  // set up first state on pins 2,6

  frequency   = 800;
  duty        =  75;
  refreshRate = 178;

  cpmCounter  = 0;
  stato       = 0;

  timer1_init();

}

int brushless::timer1_init(){

  pinMode(10,OUTPUT);
  pinMode(9,OUTPUT);
  /*
    Prescaler is configged like this:

   (1 << CS10): divide by 1, 64, 1024
   (1 << WGM13): 16 bit Phase+Frequency correct, TOP =ICR1
   (1 << COM1B1): non-inverting, and inverting?????
   */

  ICR1   = frequency;
  OCR1B  = map(duty,0,255,0,frequency);
  OCR1A  = map(duty,0,255,0,frequency);

  TCCR1B = (1 << CS10) | (1 << WGM13);
  TCCR1A = (1 << COM1B1) | (1 << COM1B0) | (1 << COM1A1) | (1 << COM1A0);

  TIMSK1 = _BV(OCIE1B);  //signal handler association
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

  int diff = val - frequency;

//    Serial.print(__FUNCTION__);
//    Serial.print(" :diff is:");
//    Serial.println(diff);

  if(diff == 0){
    Serial.print("setFrequency exit: same value ");
    Serial.println(frequency);
    return frequency;
  }
  if(diff > 0){
    for(int i=0;i<diff;i++){
      ICR1      = ++frequency;
      setDuty(duty);
 //    Serial.println(frequency);
    }
  }
  if(diff < 0){
    for(int i=diff;i<0;i++){
      ICR1      = --frequency;
      setDuty(duty);
  //    Serial.println(frequency);
    }
  }

  return ICR1;
}

int brushless::setDuty(int val){

  if(val < 0 || val >= 255) return -1;

  duty  = val;
  OCR1B = map(duty,0,255,0,frequency);
  OCR1A = map(duty,0,255,0,frequency);
  return OCR1B;

}

int brushless::setRefreshRate(int val){

  /*
  necessaria un analisi sperimentale di questo valore
   */


  int diff = val - refreshRate;

  if(diff == 0){
    Serial.print("setRefreshRate exit: same value ");
    Serial.println(refreshRate);
    return 0;
  }

  if(diff > 0){
    for(int i=0;i<diff;i++){
      refreshRate = ++refreshRate;
//      Serial.println(refreshRate);
    }
  }
  if(diff < 0){
    for(int i=diff;i<=0;i++){
      refreshRate = --refreshRate;
//      Serial.println(refreshRate);
    }
  }
  return 0;
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


