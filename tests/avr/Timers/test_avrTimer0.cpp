#include <test_avrTimer0.h>

void handlerT(uint8_t v) {
  log_info("Executing setTop on value: %d", v);
  timer0_setTop(v);
}
void handlerD(uint8_t v) {

  log_info("Executing setDuty on value: %d", v);
  timer0_setDuty(v);
}

void test_avrTimer0() {

  timer0_init(PHASE_CORRECT_OCRA, 1);
  timer0_setPin('B', CLEAR);

  //timer0_setTop(200);
  timer0_setDuty(50);
  timer0_setFrequency(35);

  timer0_start();

  comunicator pippo = comunicator();
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

      timer0_setFrequency(300);
      log_info(" set frequency 300 %" PRIu16 "", timer0_getPrescaler());
      log_info(" prescaler: %" PRIu16 ",duty: %d",timer0_getPrescaler(),timer0_getDuty());
      timeClock_wait(1500);
      timer0_setFrequency(1600000); 
      log_info(" set frequency 1600 Khz  at prescaler %" PRIu16 "", timer0_getPrescaler());
      timeClock_wait(834);
       uint32_t i;
        
      log_info("start loop");
      //for (i = 10; i <= 2000000; ) {
      for (i = 40; i <= 4000000; ) {
      //  log_info("set top to %d", i);
      //  timer0_setTop(i);
   
	      
	      timer0_setFrequency(i);
	      
	      if(i < 1000){
		      if( i % 10 == 0){
		  	log_info("freq: %" PRIu32 ", prescaler: %" PRIu16 ",duty: %" PRIu8 ,i,timer0_getPrescaler(),timer0_getDuty());
			}
			timeClock_wait(250);
		      	i = i+20;
	      }	      
	      else if(i < 59000){
	      	if( i % 100 == 0){
		  	log_info("freq: %" PRIu32 ", prescaler: %" PRIu16 ",duty: %" PRIu8 ,i,timer0_getPrescaler(),timer0_getDuty());
			}
	      	timeClock_wait(250);
	      	i = i+100;
	      }else{
	      	if( i % 1000 == 0){
		  	log_info("freq: %" PRIu32 ", prescaler: %" PRIu16 ",duty: %" PRIu8 ,i,timer0_getPrescaler(),timer0_getDuty());
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
      timer0_setPrescaler(1);

      for (i = 50; i > 0; i--) {
        log_info("set top to %" PRIu8 "", i);
        timer0_setTop(i);
  
        timeClock_wait(1000);
      }

      //	timer0_decreasePrescaler();
      //	log_info("set prescaler to %d",timer0_getPrescaler());
      //	timeClock_wait(834);
    }

    if ((PINB & (1 << PB3)) == 0) {
      timer0_setFrequency(1000000);
      log_info(" set frequency 1000000 %" PRIu16 "", timer0_getPrescaler());
      timeClock_wait(834);
      
      int i = 0;
      for(;i<10;i++){
      	timer0_setFrequency(1000000);
      	log_info("running at %" PRIu32 " prescaler: %" PRIu16 " top: %" PRIu16 " duty: ",timer0_getFrequency(), timer0_getPrescaler(), timer0_getDuty());
        timeClock_wait(834);
      
        timer0_setFrequency(35);
      	log_info("running at %" PRIu32 " prescaler: %" PRIu16 " top: %" PRIu16 " duty: ",timer0_getFrequency(), timer0_getPrescaler(), timer0_getDuty());
        timeClock_wait(834);
      
        timer0_setFrequency(2500);
      	log_info("running at %" PRIu32 " prescaler: %" PRIu16 " top: %" PRIu16 " duty: ",timer0_getFrequency(), timer0_getPrescaler(), timer0_getDuty());
        timeClock_wait(834);
      
      
      }
      
      
    }
    if ((PINB & (1 << PB2)) == 0) {
      v = timer0_getFrequency() - 50;
      timer0_setFrequency(v);
 
      log_info("set TOP to %" PRIu8 ", freq: %" PRIu32 ", clock %" PRIu32 "", v, timer0_getFrequency(),F_CPU);
      timeClock_wait(834);
    }
  }

  // return 0;
}
