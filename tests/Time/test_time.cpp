#include <test_time.h>

void test_time() {

  comunicator pippo;
  log_info("Comunicator ready..let's test!");

  log_info("initializing internal clock");

  timeClock_init();

  log_info("clock: %lu", timeClock_getMillis());
#ifdef _AVR_CLOCKTIMER_TIMER2_ 
SET_TIMER2_PINA; // digital pin 11
  
  //DDRB |= (1 << PB3); // digital pin 11
  //DDRD |= (1 << PD6); // digital pin 6

  log_info("set pin high");
  SET_TIMER2_PINA_SET;
  timeClock_wait(500);
  log_info("set pin low");
  SET_TIMER2_PINA_CLEAR;
  timeClock_wait(500);



#endif
#ifdef _AVR_CLOCKTIMER_TIMER0_ 
  SET_TIMER0_PINA; // digital pin 11
  
  //DDRB |= (1 << PB3); // digital pin 11
  //DDRD |= (1 << PD6); // digital pin 6

  log_info("set pin high");
  SET_TIMER0_PINA_SET;
  timeClock_wait(500);
  log_info("set pin low");
  SET_TIMER0_PINA_CLEAR;
  timeClock_wait(500);

#endif
  
 uint8_t blinky = 0; 
   
  //long myTime = timeClock_getMillis();
  //long oldTime = timeClock_getMillis();

  log_info("Calibrazione intervallo 1 sec.");

  while (1) {
   /*
    if (timeClock_getMillis() % 250 == 0) {
      oldTime = myTime;
      
      PINB |= (1 << PB3); // toggle pin 11
     // PIND |= (1 << PD6); // toggle pin 6
      
      log_info("time: %ld, seconds: %ld", timeClock_getMillis(),
               timeClock_getSeconds());
    }
  }*/
  log_info("looping and flapping");
  if(blinky == 0){
  
#ifdef _AVR_CLOCKTIMER_TIMER2_ 
  	SET_TIMER2_PINA_SET;
#endif
#ifdef _AVR_CLOCKTIMER_TIMER0_ 
  	SET_TIMER0_PINA_SET;
#endif
        blinky = 1;
  }else{
#ifdef _AVR_CLOCKTIMER_TIMER2_
	SET_TIMER2_PINA_CLEAR;
#endif
#ifdef _AVR_CLOCKTIMER_TIMER0_  
  	SET_TIMER0_PINA_CLEAR;
#endif
  	blinky = 0;
  }
  timeClock_wait(1000);
  
  
  }
}
