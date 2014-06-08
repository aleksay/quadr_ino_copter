#include "avrTimer0.h"

// GLOBALS
uint8_t timer0_prescaler;
uint32_t delaytime;


// FUNCTIONS
uint8_t timer0_init() {
  //configure timer0
  timer0_phasecorrect_ocr0atop_init();

  timer0_prescaler = DEFAULT_T0_INIT_PRESCALER;
  timer0_setFrequency(DEFAULT_T0_INIT_FREQUENCY); //also sets duty
  return 0 ;
}

void timer0_phasecorrect_ocr0atop_init() {
  //cli();
  SET_TIMER0_PINB;
  TIMER0_RESET;

  SET_TIMER0_PINOUT(B);
  SET_TIMER0_MODE_PHASE_CORRECT_OCR0A;
  SET_TIMER0_PINB_NOTINVERTING(0);
  //SET_TIMER0_INTERRUPT_OUTPUTCOMPARE_B;
  //SET_TIMER0_INTERRUPT_OVERFLOW;
  //sei();
}

uint8_t timer0_start() {
  timer0_setPrescaler(timer0_prescaler);

  return 0;
}
uint8_t timer0_start(uint16_t _prescaler) {
  timer0_setPrescaler(_prescaler);
  return 0;
}

//stop timer0 by removing the prescaler
uint8_t timer0_stop() {
  delaytime = avrClock();
  while ((avrClock() - delaytime) < 10)
  {
    timer0_setDuty(0);
  }// wait then clear the prescaler

  SET_TIMER0_STOP;
  return 0;
}

int8_t timer0_setPrescaler(uint16_t _prescaler) {
  debug("prescaler set to: %d", _prescaler);

  switch (_prescaler) {

    case 1:
      SET_TIMER0_PRESCALER_1;
      timer0_prescaler = 1;
      return 0;
    case 8:
      SET_TIMER0_PRESCALER_8;
      timer0_prescaler = 8;
      return 0;
    case 64:
      SET_TIMER0_PRESCALER_64;
      timer0_prescaler = 64;
      return 0;
    case 256:
      SET_TIMER0_PRESCALER_256;
      timer0_prescaler = 256;
      return 0;
    case 1024:
      SET_TIMER0_PRESCALER_1024;
      timer0_prescaler = 1024;

      return 0;
  }
  return 1;
}


int8_t timer0_setFrequency(uint16_t Hz) {

  int8_t ret;
  debug("new frequency is: %d", Hz);

  if (Hz == timer0_getFrequency()) {
    log_warn("frequency unchanged!");
    return -1;
  }

  // 15700 is minimum frequency for prescaler 1 at 8 MHz.
  if (Hz <= 15700 || Hz > 65000) {
    log_err("invalid frequency:%u", Hz);
    return -1;
  }

  // convert Hz to register value TOP
  ret = timer0_setTop( phaseCorrectPWM_Hz2Top(timer0_getPrescaler(), Hz) );
  if (ret < 0) return ret;

  // set new duty, and return error code
  ret = timer0_setDuty(timer0_getDuty());
  if (ret < 0) return ret;

  return 0;
}


int8_t timer0_setTop(uint8_t top) {

  debug("new top is: %d", top);

  if (timer0_getTop() == top) {
    log_warn("top unchanged!");
    return -1;
  }

  SET_TIMER0_OCR0A( top );
  return 0;

}

int8_t timer0_setDuty(uint8_t duty) {
  debug("new duty is: %d", duty);

  if (duty < 0 || duty > 100) {
    log_err("bad duty: %d", duty);
    return -1;
  }

  SET_TIMER0_OCR0B( avrMap(duty, 0, 100, 0, timer0_getTop()) );
  return 0;

}


uint16_t timer0_getPrescaler(void) {
  return timer0_prescaler;
}

uint16_t timer0_getFrequency(void) {
  return phaseCorrectPWM_Top2Hz(timer0_getPrescaler(), timer0_getTop());
}

uint8_t timer0_getTop(void) {
  //  return ICR0; // Depending on PWM type used
  debug("top is: %d", OCR0A);
  return OCR0A;
}

uint8_t timer0_getDuty(void) {
  return avrMap(OCR0B, 0, timer0_getTop(), 0, 100);
}





