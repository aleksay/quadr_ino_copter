#include <avrTimer1.h>

// Variables
/*
uint16_t timer1_prescaler;
uint16_t timer1_minHzPrescaler1;
uint16_t timer1_minHzPrescaler8;
uint16_t timer1_minHzPrescaler64;
uint16_t timer1_minHzPrescaler256;
*/

timer_mode timer1_mode = NORMAL;
uint16_t timer1_prescaler = 0;
volatile uint8_t timer1_bottom = 0;
volatile uint16_t timer1_top = 0;
uint8_t timer1_duty = 0;
uint8_t timer1_running = 0;
uint16_t timer1_allowedPrescalers[] = { 1, 8, 64, 256, 1024} ;
uint8_t timer1_allowedPrescalersLenght = sizeof(timer1_allowedPrescalers) / sizeof(timer1_allowedPrescalers[0]);



// declare pointer
//void (*myfunc)(void);

void (*timer1_incmp_handler)();
void (*timer1_ovf_handler)();
void (*timer1_compa_handler)();
void (*timer1_compb_handler)();

// functions
//void timer1_register_COMPA_callback(void (*func)(void)) { myfunc = func; }


void timer1_register_INCMP_callback (void (*func) (void)) { timer1_incmp_handler = func; }
void timer1_register_COMPA_callback(void (*func)(void))   { timer1_compa_handler = func; }
void timer1_register_COMPB_callback(void (*func)(void))   { timer1_compb_handler = func; }
void timer1_register_OVF_callback(void (*func)(void))     { timer1_ovf_handler = func;   }


ISR(TIMER1_COMPA_vect) { (*timer1_compa_handler)(); }
ISR(TIMER1_COMPB_vect) { (*timer1_compb_handler)(); }
ISR(TIMER1_OVF_vect)   { (*timer1_ovf_handler)();   }
ISR(TIMER1_CAPT_vect)  { (*timer1_incmp_handler)(); }



// FUNCTIONS
int8_t timer1_init() {

  // calculate max values per prescaler
//  timer1_getPrescalerMinHz();

  // configure timer1
  timer1_mode_fastpwm_ocra_init();
  //set Pin B ONCOMPARE

  return 0;
}

void timer1_init(timer_mode _mode, uint16_t _prescaler){


  timer1_stop();

  if (_mode == NORMAL) {
    timer1_mode = _mode;
    timer1_mode_normal_init();
    timer1_setPrescaler(_prescaler);
    timer1_setBottom(256 - (int)((float)F_CPU * 0.001 / timer1_prescaler));
    // da usare con ISR(TIMER0_OVF_vect)
  }
  if (_mode == PHASE_CORRECT_OCRA) {

    timer1_mode = _mode;
    timer1_mode_phasecorrect_ocra_init();

    timer1_setPrescaler(_prescaler);
    // timer0_setFrequency(0); //also sets duty
  }
  if (_mode == CTC_OCRA) {
    timer1_mode = _mode;
    timer1_mode_ctc_ocra_init();
    timer1_setPrescaler(_prescaler);
    // da usare con timer0_register_COMPA_callback
  }
  if (_mode == FASTPWM_OCRA){
    timer1_mode = _mode;
    timer1_mode_fastpwm_ocra_init();
    // set pin B
    timer1_setPrescaler(_prescaler);
  
  }
  if (_mode == FASTPWM_ICR){
    timer1_mode = _mode;
    timer1_mode_fastpwm_icr_init();
    // set interrupt
    // set pin B 
    // initial values
    timer1_setPrescaler(_prescaler);
  }
  if (_mode == PHASE_CORRECT_TOP) {}
  if (_mode == FASTPWM_TOP){}

}

void timer1_mode_normal_init(){

  UNSET_TIMER1_PINA;
  UNSET_TIMER1_PINB;

  TIMER1_RESET;

  SET_TIMER1_MODE_NORMAL;

}
void timer1_mode_phasecorrect_ocra_init(){

  UNSET_TIMER1_PINA;
  UNSET_TIMER1_PINB;

  TIMER1_RESET;

  SET_TIMER1_MODE_PHASE_CORRECT_OCRA;

}
void timer1_mode_ctc_ocra_init(){

  UNSET_TIMER1_PINA;
  UNSET_TIMER1_PINB;

  TIMER1_RESET;
  // SET_TIMER0_CLOCK_INTERNAL;
  SET_TIMER1_MODE_CTC_OCRA;

}
void timer1_mode_phasecorrect_top_init(){}
void timer1_mode_fastpwm_top_init(){}




void timer1_mode_fastpwm_icr_init() {
  
  UNSET_TIMER1_PINA;
  UNSET_TIMER1_PINB;
  TIMER1_RESET;
    
  // set interrupt
  //SET_TIMER1_INTERRUPT_OUTPUTCOMPARE_B;
  
  // set pin B 
  //SET_TIMER1_PINB;
  //SET_TIMER1_PINB_CLEAR_ONCOMPARE;
  
  // initial values
  //SET_TIMER1_ICR(DEFAULT_T1_INIT_FREQUENCY);
  //SET_TIMER1_OCRB(DEFAULT_T1_INIT_DUTY);
   
  
  SET_TIMER1_MODE_FASTPWM_ICR;
  
 // return 0;
}
     
void timer1_mode_fastpwm_ocra_init() {
/* ORIG:
  SET_TIMER1_PINB;
  SET_TIMER1_PINB_CLEAR_ONCOMPARE;
  SET_TIMER1_MODE_FASTPWM_OCRA;
  return 0;

*/

  UNSET_TIMER1_PINA;
  UNSET_TIMER1_PINB;
  TIMER1_RESET;

  // set Pin B 
//  SET_TIMER1_PINB;
//  SET_TIMER1_PINB_CLEAR_ONCOMPARE;
   
  SET_TIMER1_MODE_FASTPWM_OCRA;
}



void timer1_clearPin(char _pin) {
  if (_pin == 'A') {
    SET_TIMER1_PINA_DISABLE;
    SET_TIMER1_PINA_CLEAR;
    UNSET_TIMER1_PINA;
  }
  if (_pin == 'B') {
    SET_TIMER1_PINB_DISABLE;
    SET_TIMER1_PINB_CLEAR;
    UNSET_TIMER1_PINB;
  }
}

void timer1_setPin(char _pin, timer_pin_mode _mode) {

  if (_pin == 'A') {
    SET_TIMER1_PINA;
    SET_TIMER1_PINA_CLEAR;

    if (_mode == TOGGLE) {
      SET_TIMER1_PINA_TOGGLE_ONCOMPARE;
    }
    if (_mode == CLEAR) {
      SET_TIMER1_PINA_CLEAR_ONCOMPARE;
    }
    if (_mode == SET) {
      SET_TIMER1_PINA_SET_ONCOMPARE;
    }
  }

  if (_pin == 'B') {
    SET_TIMER1_PINB;
    SET_TIMER1_PINB_CLEAR;

    if (_mode == TOGGLE) {
      SET_TIMER1_PINB_TOGGLE_ONCOMPARE;
    }
    if (_mode == CLEAR) {
      SET_TIMER1_PINB_CLEAR_ONCOMPARE;
    }
    if (_mode == SET) {
      SET_TIMER1_PINB_SET_ONCOMPARE;
    }
  }
}

uint8_t timer1_isRunning() { return timer1_running; }


/*
void timer1_getPrescalerMinHz(void) {
  timer1_minHzPrescaler1 = fastPWM_Top2Hz(1, UINT16_MAX) + 1;
  timer1_minHzPrescaler8 = fastPWM_Top2Hz(8, UINT16_MAX) + 1;
  timer1_minHzPrescaler64 = fastPWM_Top2Hz(64, UINT16_MAX) + 1;
  timer1_minHzPrescaler256 = fastPWM_Top2Hz(256, UINT16_MAX) + 1;

  //  debug("timer1_minHzPrescaler1:%u", timer1_minHzPrescaler1);
  //  debug("timer1_minHzPrescaler8:%u", timer1_minHzPrescaler8);
  //  debug("timer1_minHzPrescaler64:%u", timer1_minHzPrescaler64);
  //  debug("timer1_minHzPrescaler256:%u", timer1_minHzPrescaler256);
}*/
/*
uint16_t timer1_getPrescalerRequired(uint32_t Hz) {

  if (Hz >= timer1_minHzPrescaler1)
    return 1;
  if (Hz >= timer1_minHzPrescaler8)
    return 8;
  if (Hz >= timer1_minHzPrescaler64)
    return 64;
  if (Hz >= timer1_minHzPrescaler256)
    return 256;
  return 0;
}
*/





void _timer1_setPrescaler(uint16_t _prescaler) {
  switch (_prescaler) {
  case 1:
    SET_TIMER1_PRESCALER_1;
    return;
  case 8:
    SET_TIMER1_PRESCALER_8;
    return;
  case 64:
    SET_TIMER1_PRESCALER_64;
    return;
  case 256:
    SET_TIMER1_PRESCALER_256;
    return;
  case 1024:
    SET_TIMER1_PRESCALER_1024;
    return;
  default:
    return;
  }
}

void timer1_setPrescaler(uint16_t _prescaler) {

  uint8_t i;
  for (i = 0; i < timer1_allowedPrescalersLenght; i++) {
    if (_prescaler == timer1_allowedPrescalers[i]) {
      timer1_prescaler = _prescaler;
      if (timer1_isRunning()) {
        _timer1_setPrescaler(_prescaler);
      }
    }
  }
}






uint16_t timer1_getPrevPrescaler() {

  uint8_t i;
  for (i = 0; i < timer1_allowedPrescalersLenght; i++) {
    if (timer1_prescaler == timer1_allowedPrescalers[i]) {
      return timer1_allowedPrescalers[(i - 1) % timer1_allowedPrescalersLenght];
    }
  }
}

uint16_t timer1_getNextPrescaler() {

  uint8_t i;
  for (i = 0; i < timer1_allowedPrescalersLenght; i++) {
    if (timer1_prescaler == timer1_allowedPrescalers[i]) {
      return timer1_allowedPrescalers[(i + 1) % timer1_allowedPrescalersLenght];
    }
  }
}

void timer1_increasePrescaler() {
  timer1_setPrescaler(timer1_getNextPrescaler());
}
void timer1_decreasePrescaler() {
  timer1_setPrescaler(timer1_getPrevPrescaler());
}

uint16_t timer1_getRequiredPrescaler(uint32_t Hz) {

  uint8_t i;
  for (i = 0; i < timer1_allowedPrescalersLenght; i++) {
    if (Hz >= prescalerMinHz(timer1_mode, timer1_allowedPrescalers[i], 16)) {
      return timer1_allowedPrescalers[i];
    }
  }
}

void timer1_resetCounter() {

// FASTPWM_ICR,FASTPWM_TOP,PHASE_CORRECT_TOP

  if (timer1_mode == NORMAL || timer1_mode == FASTPWM_ICR || timer1_mode == FASTPWM_TOP || timer1_mode == PHASE_CORRECT_TOP) {
    cli();
    TCNT1 = timer1_bottom;
    sei();
  }
  if (timer1_mode == CTC_OCRA || timer1_mode == FASTPWM_OCRA || timer1_mode == PHASE_CORRECT_OCRA) {
    cli();
    TCNT1 = 0;
    sei();
  }
}



void timer1_start(uint32_t Hz) {
  // timer1_setPrescaler(DEFAULT_T1_INIT_PRESCALER);
  timer1_setFrequency(Hz);
  timer1_start();
}

void timer1_start() { timer1_start(timer1_prescaler); }

void timer1_start(uint16_t _prescaler) {
  timer1_resetCounter();

  if (timer1_mode == NORMAL) {
    
    SET_TIMER1_INTERRUPT_OVERFLOW;
  }
  if (timer1_mode == CTC_OCRA) {
    
    SET_TIMER1_INTERRUPT_OUTPUTCOMPARE_A;
  }
  if (timer1_mode == FASTPWM_OCRA) {
  }
  if (timer1_mode == FASTPWM_ICR) {               
  }
  if (timer1_mode == FASTPWM_TOP) {
  }
  if (timer1_mode == PHASE_CORRECT_OCRA) {
  }
  if (timer1_mode == PHASE_CORRECT_TOP) {
  }
  
  timer1_running = 1;
  timer1_setPrescaler(_prescaler);
}




// stop timer1 by removing the prescaler
void timer1_stop() {
 // SET_TIMER1_PRESCALER_0;
  //  pins_setDriveOpenInverter(); // TODO remove me
  
  if (timer1_mode == NORMAL) {
    UNSET_TIMER1_INTERRUPT_OVERFLOW;    
  }
  if (timer1_mode == CTC_OCRA) {
    UNSET_TIMER1_INTERRUPT_OUTPUTCOMPARE_A;    
  }
  if (timer1_mode == FASTPWM_OCRA) {
  }
  if (timer1_mode == FASTPWM_ICR) {
  }
  if (timer1_mode == FASTPWM_TOP) {
  }
  if (timer1_mode == PHASE_CORRECT_OCRA) {
  }
  if (timer1_mode == PHASE_CORRECT_TOP) {
  }
  
  timer1_running = 0;
  _timer1_setPrescaler(0);
  
  
}







int8_t timer1_setFrequency(uint32_t Hz) {

  int8_t ret;

  // check Hz
  // TODO implement logging rate limiting
  if (Hz == timer1_getFrequency()) {
    //    log_warn("frequency unchanged! Hz:%u", Hz);
    return -1;
  }

  // update prescaler if necessary
 // if (timer1_getPrescalerRequired(Hz) != timer1_getPrescaler()) {
 //   timer1_setPrescaler(timer1_getPrescalerRequired(Hz));
 // }
  // debug("Hz:%u, Required prescaler:%u", Hz, timer1_getPrescalerRequired(Hz));

  // convert Hz to register value TOP
  ret = timer1_setTop(fastPWM_Hz2Top(timer1_getPrescaler(), Hz));
  if (ret < 0)
    return ret;

  // debug("Top:%u",fastPWM_Hz2Top(timer1_getPrescaler(),Hz));

  // set new duty, and return error code
  ret = timer1_setDuty(timer1_getDuty());
  if (ret < 0)
    return ret;

  return 0;
}

int8_t timer1_setTop(uint16_t top) {

  if (timer1_getTop() == top) {
    //    log_warn("top unchanged!");
    return -1;
  }

  // set new TOP value in register
  SET_TIMER1_OCRA(top);
  return 0;
}

void timer1_setBottom(uint8_t _bottom) { timer1_bottom = _bottom; }

int8_t timer1_setDuty(uint8_t duty) {

  if (duty <= 0 || duty > 100) {
    //    log_err("bad duty: %d", duty);
    return -1;
  }

  SET_TIMER1_OCRB(avrMap(duty, 0, 100, 0, timer1_getTop()));
  // SET_TIMER1_DUTY_CHAN_B( map(duty, 1, 100, 1, timer1_getTop()) );
  return 0;
}

uint16_t timer1_getPrescaler(void) { return timer1_prescaler; }

uint16_t timer1_getFrequency(void) {
  return fastPWM_Top2Hz(timer1_getPrescaler(), timer1_getTop());
}

uint16_t timer1_getTop(void) {
  //  return ICR1; // Depending on PWM type used
  return GET_TIMER1_OCRA();
}

uint16_t timer1_getDuty(void) {
  return avrMap(GET_TIMER1_OCRB(), 0, timer1_getTop(), 0, 100);
  //      return map(OCR1B, 0, timer1_getTop(), 0, 100);
}

void timer1_timer1_ovf_handler(void) {
  SET_TIMER1_OCRA(timer1_getFrequency());
  SET_TIMER1_OCRB(timer1_getDuty());
}


