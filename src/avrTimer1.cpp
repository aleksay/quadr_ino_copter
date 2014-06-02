
#include "avrTimer1.h"

// Variables
uint16_t timer1_prescaler;
uint16_t timer1_minHzPrescaler1;
uint16_t timer1_minHzPrescaler8;
uint16_t timer1_minHzPrescaler64;
uint16_t timer1_minHzPrescaler256;


// FUNCTIONS
int8_t timer1_init() {

  // calculate max values per prescaler
  timer1_getPrescalerMinHz();

  // configure timer1
  timer1_fastPwm_ocr1atop_init();

  // initialize timer1 global variable
  timer1_prescaler = DEFAULT_T1_INIT_PRESCALER;
  timer1_setFrequency(DEFAULT_T1_INIT_FREQUENCY); //also sets duty


  return 0;
}

int8_t timer1_fastPwm_icr1top_init() {
  SET_TIMER1_PINB;
  SET_TIMER1_FREQUENCY_ICR1TOP(DEFAULT_T1_INIT_FREQUENCY);
  SET_TIMER1_DUTY_CHAN_B(DEFAULT_T1_INIT_DUTY);
  TIMER1_RESET;
  SET_TIMER1_PINOUT(B);
  SET_TIMER1_MODE_FASTPWM_ICR1;
  SET_TIMER1_PINB_NOTINVERTING(0);
  SET_TIMER1_INTERRUPT_OUTPUTCOMPARE_B;
  //SET_TIMER1_PRESCALER_1;
  return 0;
}

int8_t timer1_fastPwm_ocr1atop_init() {
  SET_TIMER1_INTERRUPT_OUTPUTCOMPARE_A;
  //SET_TIMER1_INTERRUPT_OUTPUTCOMPARE_B;
  //SET_TIMER1_INTERRUPT_OVERFLOW;
  TIMER1_RESET;
  SET_TIMER1_PINB;
  //SET_TIMER1_PINOUT(B);
  SET_TIMER1_MODE_FASTPWM_OCR1A;
  SET_TIMER1_PINB_NOTINVERTING(0);
  return 0;
}

void timer1_start() {
  timer1_setPrescaler(DEFAULT_T1_INIT_PRESCALER);
  timer1_setFrequency(DEFAULT_T1_INIT_FREQUENCY);
}

//stop timer1 by removing the prescaler
void timer1_stop() {
  SET_TIMER1_STOP;
}

void timer1_getPrescalerMinHz(void) {
  timer1_minHzPrescaler1 =   timer1_Top2Hz(1  , UINT16_MAX) + 1;
  timer1_minHzPrescaler8 =   timer1_Top2Hz(8  , UINT16_MAX) + 1;
  timer1_minHzPrescaler64 =  timer1_Top2Hz(64 , UINT16_MAX) + 1;
  timer1_minHzPrescaler256 = timer1_Top2Hz(256, UINT16_MAX) + 1;

  debug("timer1_minHzPrescaler1:%u", timer1_minHzPrescaler1);
  debug("timer1_minHzPrescaler8:%u", timer1_minHzPrescaler8);
  debug("timer1_minHzPrescaler64:%u", timer1_minHzPrescaler64);
  debug("timer1_minHzPrescaler256:%u", timer1_minHzPrescaler256);

}

//convert to TOP value for calculating register value
uint16_t timer1_Hz2Top(uint16_t prescaler, uint16_t Hz) {
  //debug("%lu,%u,%u",F_CPU,prescaler,Hz);
  return floor(F_CPU / (prescaler * Hz) - 1);
}

// used for calculating max TOP, min frequency
uint16_t timer1_Top2Hz(uint16_t prescaler, uint16_t top) {
  //debug("%lu,%u,%u", F_CPU, prescaler, top);
  return F_CPU /  (top + 1) / prescaler;
}

uint16_t timer1_getPrescalerRequired(uint16_t Hz) {



  if (Hz >= timer1_minHzPrescaler1)
    return 1;
  if (Hz >= timer1_minHzPrescaler8)
    return 8;
  if (Hz >= timer1_minHzPrescaler64)
    return 64;
  if (Hz >= timer1_minHzPrescaler256)
    return 256;
}


int8_t timer1_setPrescaler(uint16_t _prescaler) {
  debug("prescaler set to: %d", _prescaler);
  switch (_prescaler) {
    case 1:
      SET_TIMER1_PRESCALER_1;
      timer1_prescaler = 1;
      return 0;
    case 8:
      SET_TIMER1_PRESCALER_8;
      timer1_prescaler = 8;
      return 0;
    case 64:
      SET_TIMER1_PRESCALER_64;
      timer1_prescaler = 64;
      return 0;
    case 256:
      SET_TIMER1_PRESCALER_256;
      timer1_prescaler = 256;
      return 0;
    case 1024:
      SET_TIMER1_PRESCALER_1024;
      timer1_prescaler = 1024;
      return 0;
  }

  return 1;
}

int8_t timer1_setFrequency(uint16_t Hz) {

  int8_t ret;

  // check Hz
  if (Hz == timer1_getFrequency()) {
    log_warn("frequency unchanged!");
    return -1;
  }

  // update prescaler if necessary
  if (timer1_getPrescalerRequired(Hz) != timer1_getPrescaler() ) {
    timer1_setPrescaler(timer1_getPrescalerRequired(Hz));
  }
  debug("Hz:%u, Required prescaler:%u", Hz, timer1_getPrescalerRequired(Hz));

  // convert Hz to register value TOP
  ret = timer1_setTop( timer1_Hz2Top(timer1_getPrescaler(), Hz) );
  if (ret < 0) return ret;

  //debug("Top:%u",timer1_Hz2Top(timer1_getPrescaler(),Hz));

  // set new duty, and return error code
  ret = timer1_setDuty(timer1_getDuty());
  if (ret < 0) return ret;

  return 0;
}

int8_t timer1_setTop(uint16_t top) {

  if (timer1_getTop() == top) {
    log_warn("top unchanged!");
    return -1;
  }

  // set new TOP value in register
  SET_TIMER1_FREQUENCY_OCR1ATOP(top);
  return 0;
}

int8_t timer1_setDuty(uint8_t duty) {

  if (duty < 0 || duty > 100) {
    log_err("bad duty");
    return -1;
  }

  SET_TIMER1_DUTY_CHAN_B( avrMap(duty, 0, 100, 0, timer1_getTop()) );
  return 0;

}

uint16_t timer1_getPrescaler(void) {
  return timer1_prescaler;
}

uint16_t timer1_getFrequency(void) {
  return timer1_Top2Hz(timer1_getPrescaler(), timer1_getTop());
}

uint16_t timer1_getTop(void) {
  //  return ICR1; // Depending on PWM type used
  return OCR1A;
}

uint16_t timer1_getDuty(void) {
  return avrMap(OCR1B, 0, 255, 0, 100);
}

void timer1_timer1_ovf_handler(void) {
  SET_TIMER1_FREQUENCY_OCR1ATOP(timer1_getFrequency());
  SET_TIMER1_DUTY_CHAN_B(timer1_getDuty());
}

