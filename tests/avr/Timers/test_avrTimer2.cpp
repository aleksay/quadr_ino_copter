#include <test_avrTimer2.h>

/*

*/
volatile uint32_t myMillis = 0;
volatile uint32_t mySeconds = 0;
// volatile unsigned char myCallback_lock=0;
volatile uint32_t dynAdjusting = 0;

void my_callback() {

  myMillis++;

  if ((myMillis - dynAdjusting) >= 10) {

    dynAdjusting = myMillis - (myMillis - dynAdjusting - 10);
  }
  if (myMillis >= 1000) {
    myMillis = 0;
    dynAdjusting = 0;
    mySeconds++;
  }
}

void test_avrTimer2() {

  comunicator pippo;

  // #define timeClock_timeout 4294967295

  log_info("timer init ctc ocra");

  timer2_init(CTC_OCRA, 128);
  timer2_setTop(124);
  // timer2_register_OVF_callback(timeNop);
  timer2_register_COMPA_callback(my_callback);
  timer2_setPin('B', CLEAR);
  timer2_start();

  long myTime = myMillis;
  long oldTime = myMillis;

  log_info("Calibrazione intervallo 1 sec. millis start %ld", oldTime);

  while (1) {
    // log_info("looping %ld",myMillis);
    myTime = myMillis;
    if (oldTime > myTime) {
      oldTime = myTime;
    }
    if ((myTime - oldTime) >= 500) {

      log_info("timeout! %ld %ld %ld %ld", oldTime, myTime, dynAdjusting,
               mySeconds);
      oldTime = myTime; // - (myTime - oldTime - 500);
    }
  }

  /*
          timer2_init(NORMAL,128);
          timer2_setTop(timeClock_timeout);
          timer2_register_OVF_callback(myTimeNop);

          timer2_start();

          long myTime = timer2_getOvfCounter();
          long oldTime = timer2_getOvfCounter();

          log_info("Calibrazione intervallo 1 sec.");

          while(1){

                  myTime  = timer2_getOvfCounter();
                  if ((myTime - oldTime) >= 1000){
                          oldTime = myTime;

                          log_info("timeout! %ld",oldTime);
                  }
            }
  */
}



void test_avrTimer2_extended(){



  timer2_init(PHASE_CORRECT_OCRA, 1);
  timer2_setPin('B', CLEAR);

  //timer2_setTop(200);
  timer2_setDuty(50);
  timer2_setFrequency(35);

  timer2_start();

  comunicator pippo = comunicator();
  
  //da lanciare con _AVR_CLOCKTIMER_TIMER0_
  timeClock_init();

  DDRB &= ~((1 << PB5) | (1 << PB4) | (1 << PB3) |
            (1 << PB2)); // Imposta PB5 come input
  PORTB |= (1 << PB5) | (1 << PB4) | (1 << PB3) |
           (1 << PB2); // Attiva la resistenza di pull-up

  uint32_t v = 1;

  log_info("waiting to start..");
  // while(timeClock() % 500 != 0){}

  // fprintf(stdout,"Comunicator ready, waiting for a b command...");
  //log_info("start loop");
  while (1) {

    if ((PINB & (1 << PB5)) == 0) {

      timer2_setFrequency(300);
      log_info(" set frequency 300 %" PRIu16 "", timer2_getPrescaler());
      log_info(" prescaler: %" PRIu16 ",duty: %d",timer2_getPrescaler(),timer2_getDuty());
      timeClock_wait(1500);
      timer2_setFrequency(1600000); 
      log_info(" set frequency 1600 Khz  at prescaler %" PRIu16 "", timer2_getPrescaler());
      timeClock_wait(834);
       uint32_t i;
        
      log_info("start loop");
      //for (i = 10; i <= 2000000; ) {
      for (i = 40; i <= 4000000; ) {
      //  log_info("set top to %d", i);
      //  timer2_setTop(i);
   
	      
	      timer2_setFrequency(i);
	      
	      if(i < 1000){
		      if( i % 10 == 0){
		  	log_info("freq: %" PRIu32 ", prescaler: %" PRIu16 ",duty: %" PRIu8 ,i,timer2_getPrescaler(),timer2_getDuty());
			}
			timeClock_wait(250);
		      	i = i+20;
	      }	      
	      else if(i < 59000){
	      	if( i % 100 == 0){
		  	log_info("freq: %" PRIu32 ", prescaler: %" PRIu16 ",duty: %" PRIu8 ,i,timer2_getPrescaler(),timer2_getDuty());
			}
	      	timeClock_wait(250);
	      	i = i+100;
	      }else{
	      	if( i % 1000 == 0){
		  	log_info("freq: %" PRIu32 ", prescaler: %" PRIu16 ",duty: %" PRIu8 ,i,timer2_getPrescaler(),timer2_getDuty());
			}
	      	timeClock_wait(250);
	      	i = i+1000;
	      
	      }
	      
	      
      }
      log_info("loop completed.");
      
    }

    if ((PINB & (1 << PB4)) == 0) {
      log_info("starting sequnece");
      int i;
      timer2_setPrescaler(1);

      for (i = 50; i > 0; i--) {
        log_info("set top to %" PRIu8 "", i);
        timer2_setTop(i);
  
        timeClock_wait(1000);
      }

      //	timer2_decreasePrescaler();
      //	log_info("set prescaler to %d",timer2_getPrescaler());
      	timeClock_wait(834);
    }

    if ((PINB & (1 << PB3)) == 0) {
      timer2_setFrequency(1000000);
      log_info(" set frequency 1000000 %" PRIu16 "", timer2_getPrescaler());
      timeClock_wait(834);
      
      int i = 0;
      for(;i<10;i++){
      	timer2_setFrequency(1000000);
      	log_info("running at %" PRIu32 " prescaler: %" PRIu16 " top: %" PRIu16 " duty: ",timer2_getFrequency(), timer2_getPrescaler(), timer2_getDuty());
        timeClock_wait(834);
      
        timer2_setFrequency(35);
      	log_info("running at %" PRIu32 " prescaler: %" PRIu16 " top: %" PRIu16 " duty: ",timer2_getFrequency(), timer2_getPrescaler(), timer2_getDuty());
        timeClock_wait(834);
      
        timer2_setFrequency(2500);
      	log_info("running at %" PRIu32 " prescaler: %" PRIu16 " top: %" PRIu16 " duty: ",timer2_getFrequency(), timer2_getPrescaler(), timer2_getDuty());
        timeClock_wait(834);
      
      
      }
      
      
    }
    if ((PINB & (1 << PB2)) == 0) {
      v = timer2_getFrequency() - 50;
      timer2_setFrequency(v);
 
      log_info("set TOP to %" PRIu8 ", freq: %" PRIu32 ", clock %" PRIu32 "", v, timer2_getFrequency(),F_CPU);
      timeClock_wait(834);
    }
  }

  // return 0;












}


















