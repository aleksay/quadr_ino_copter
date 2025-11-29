#include <avrTimer0.h>

void (*timer0_ovf_handler)();
void (*timer0_compa_handler)();
void (*timer0_compb_handler)();

uint16_t timer0_allowedPrescalers[] = { 1, 8, 64, 256, 1024};
uint8_t timer0_allowedPrescalersLenght =
    sizeof(timer0_allowedPrescalers) / sizeof(timer0_allowedPrescalers[0]);

// Var declarations
timer_mode timer0_mode = NORMAL;
uint16_t timer0_prescaler = 0;
volatile uint8_t timer0_bottom = 0;
volatile uint16_t timer0_top = 0;
uint8_t timer0_duty = 0;
uint8_t timer0_running = 0; // 0 FALSE; 1 TRUE

uint8_t timer0_isRunning() { return timer0_running; }

void timer0_init() {
  timer0_stop();
  timer0_mode_normal_init();
  timer0_setPrescaler((uint16_t)128);
  timer0_setBottom(256 - (int)((float)F_CPU * 0.001 / timer0_prescaler));
}

void timer0_init(timer_mode _mode, uint16_t _prescaler) {

  timer0_stop();

  if (_mode == NORMAL) {
    timer0_mode = _mode;
    timer0_mode_normal_init();
    timer0_setPrescaler(_prescaler);
    timer0_setBottom(256 - (int)((float)F_CPU * 0.001 / timer0_prescaler));
    // da usare con ISR(TIMER0_OVF_vect)
  }
  if (_mode == PHASE_CORRECT_OCRA) {

    timer0_mode = _mode;
    timer0_mode_phasecorrect_ocra_init();

    timer0_setPrescaler(_prescaler);
    // timer0_setFrequency(0); //also sets duty
  }
  if (_mode == CTC_OCRA) {
    timer0_mode = _mode;
    timer0_mode_ctc_ocra_init();
    timer0_setPrescaler(_prescaler);
    // da usare con timer0_register_COMPA_callback
  }
  if (_mode == FASTPWM_OCRA){
    timer0_mode = _mode;
    timer0_mode_fastpwm_ocra_init();
    // set pin B
    timer0_setPrescaler(_prescaler);
  
  }
}

void timer0_mode_normal_init() {

  UNSET_TIMER0_PINA;
  UNSET_TIMER0_PINB;

  TIMER0_RESET;

  SET_TIMER0_MODE_NORMAL;
  // SET_TIMER0_CLOCK_INTERNAL;
  // UNSET_TIMER0_INTERRUPT_OUTPUTCOMPARE_A;
}
void timer0_mode_ctc_ocra_init() {

  UNSET_TIMER0_PINA;
  UNSET_TIMER0_PINB;

  TIMER0_RESET;
  // SET_TIMER0_CLOCK_INTERNAL;
  SET_TIMER0_MODE_CTC_OCRA;
}
void timer0_mode_phasecorrect_ocra_init() {

  UNSET_TIMER0_PINA;
  UNSET_TIMER0_PINB;

  TIMER0_RESET;

  SET_TIMER0_MODE_PHASE_CORRECT_OCRA;
}

void timer0_mode_phasecorrect_top_init(){}
void timer0_mode_fastpwm_top_init(){}
void timer0_mode_fastpwm_ocra_init(){

  UNSET_TIMER0_PINA;
  UNSET_TIMER0_PINB;

  TIMER0_RESET;

//  SET_TIMER0_PINB;
 
//  SET_TIMER0_PINOUT(B);
//  SET_TIMER0_PINB_NOTINVERTING(0);

  SET_TIMER0_MODE_FASTPWM_OCRA;

}

void timer0_clearPin(char _pin) {
  if (_pin == 'A') {
    SET_TIMER0_PINA_DISABLE;
    SET_TIMER0_PINA_CLEAR;
    UNSET_TIMER0_PINA;
  }
  if (_pin == 'B') {
    SET_TIMER0_PINB_DISABLE;
    SET_TIMER0_PINB_CLEAR;
    UNSET_TIMER0_PINB;
  }
}

void timer0_setPin(char _pin, timer_pin_mode _mode) {

  if (_pin == 'A') {
    SET_TIMER0_PINA;
    SET_TIMER0_PINA_CLEAR;

    if (_mode == TOGGLE) {
      SET_TIMER0_PINA_TOGGLE_ONCOMPARE;
    }
    if (_mode == CLEAR) {
      SET_TIMER0_PINA_CLEAR_ONCOMPARE;
    }
    if (_mode == SET) {
      SET_TIMER0_PINA_SET_ONCOMPARE;
    }
  }

  if (_pin == 'B') {
    SET_TIMER0_PINB;
    SET_TIMER0_PINB_CLEAR;

    if (_mode == TOGGLE) {
      SET_TIMER0_PINB_TOGGLE_ONCOMPARE;
    }
    if (_mode == CLEAR) {
      SET_TIMER0_PINB_CLEAR_ONCOMPARE;
    }
    if (_mode == SET) {
      SET_TIMER0_PINB_SET_ONCOMPARE;
    }
  }
}



void _timer0_setPrescaler(uint16_t _prescaler) {

  switch (_prescaler) {

  case 0:
    SET_TIMER0_PRESCALER_0;
    return;
  case 1:
    SET_TIMER0_PRESCALER_1;
    return;
  case 8:
    SET_TIMER0_PRESCALER_8;
    return;
  case 64:
    SET_TIMER0_PRESCALER_64;
    return;
  case 256:
    SET_TIMER0_PRESCALER_256;
    return;
  case 1024:
    SET_TIMER0_PRESCALER_1024;
    return;
  default:
    return;
  }
}

void timer0_setPrescaler(uint16_t _prescaler) {

  int i;
  for (i = 0; i < timer0_allowedPrescalersLenght; i++) {
    if (_prescaler == timer0_allowedPrescalers[i]) {
      timer0_prescaler = _prescaler;
      if (timer0_isRunning()) {
        _timer0_setPrescaler(_prescaler);
      }
    }
  }
}

uint16_t timer0_getPrevPrescaler() {

  int i;
  for (i = 0; i < timer0_allowedPrescalersLenght; i++) {
    if (timer0_prescaler == timer0_allowedPrescalers[i]) {
      return timer0_allowedPrescalers[(i - 1) % timer0_allowedPrescalersLenght];
    }
  }
}

uint16_t timer0_getNextPrescaler() {

  int i;
  for (i = 0; i < timer0_allowedPrescalersLenght; i++) {
    if (timer0_prescaler == timer0_allowedPrescalers[i]) {
      return timer0_allowedPrescalers[(i + 1) % timer0_allowedPrescalersLenght];
    }
  }
}

void timer0_increasePrescaler() {
  timer0_setPrescaler(timer0_getNextPrescaler());
}
void timer0_decreasePrescaler() {
  timer0_setPrescaler(timer0_getPrevPrescaler());
}

uint16_t timer0_getRequiredPrescaler(uint32_t Hz) {

  int i;
  for (i = 0; i < timer0_allowedPrescalersLenght; i++) {
    if (Hz >= prescalerMinHz(timer0_mode, timer0_allowedPrescalers[i], 8)) {
      return timer0_allowedPrescalers[i];
    }
  }
}

void timer0_resetCounter() {
  if (timer0_mode == NORMAL) {
    TCNT0 = timer0_bottom;
  }
  if (timer0_mode == CTC_OCRA || timer0_mode == FASTPWM_OCRA || timer0_mode == PHASE_CORRECT_OCRA) {
    TCNT0 = 0;
  }
}

void timer0_start() { timer0_start(timer0_prescaler); }

void timer0_start(uint16_t _prescaler) {
  if (timer0_mode == NORMAL) {
    timer0_resetCounter();
    SET_TIMER0_INTERRUPT_OVERFLOW;
  }
  if (timer0_mode == CTC_OCRA) {
    timer0_resetCounter();
    SET_TIMER0_INTERRUPT_OUTPUTCOMPARE_A;
  }
  if (timer0_mode == PHASE_CORRECT_OCRA) {
  }
  if (timer0_mode == FASTPWM_OCRA) {
  }
  
  timer0_running = 1;
  timer0_setPrescaler(_prescaler);
}

void timer0_stop() {
  if (timer0_mode == NORMAL) {
    UNSET_TIMER0_INTERRUPT_OVERFLOW;    
  }
  if (timer0_mode == CTC_OCRA) {
    UNSET_TIMER0_INTERRUPT_OUTPUTCOMPARE_A;    
  }
  if (timer0_mode == PHASE_CORRECT_OCRA) {    
  }
  if (timer0_mode == FASTPWM_OCRA) {
  }
  _timer0_setPrescaler(0);
  timer0_running = 0;
}

void timer0_setTop(uint16_t _top) {

 // uint8_t old_duty = timer0_getDuty();

  if (_top <= 2)
    timer0_top = 2;
  else
    timer0_top = _top;

  // IF mode: normal -> timer0_top represent the maximum number of
  // TIMER0_OVF_vect before executing timer0_ovf_handler
  if (timer0_mode == NORMAL) {
  }
  if (timer0_mode == PHASE_CORRECT_OCRA || timer0_mode == CTC_OCRA || timer0_mode == FASTPWM_OCRA) {

    SET_TIMER0_OCRA(timer0_top);
    timer0_setDuty(timer0_duty);
  }

  
}

void timer0_setBottom(uint8_t _bottom) { timer0_bottom = _bottom; }

// from 31 Hz to 2 MHz
void timer0_setFrequency(uint32_t Hz) {

  // 15700 is minimum frequency for prescaler 1 at 8 MHz.
  //  if (Hz <= 15700 || Hz > 65000) {
  //    return;
  //  }

  // convert Hz to register value TOP
  //  timer0_setTop( phaseCorrectPWM_Hz2Top(timer0_getPrescaler(), Hz) );
  //  timer0_setDuty(timer0_getDuty());

  if (timer0_mode == PHASE_CORRECT_OCRA || timer0_mode == PHASE_CORRECT_TOP) {

    timer0_setPrescaler(timer0_getRequiredPrescaler(Hz));
    timer0_setTop(phaseCorrectPWM_Hz2Top(timer0_getPrescaler(), Hz));
  }
  if (timer0_mode == FASTPWM_OCRA || timer0_mode == FASTPWM_TOP) {

    timer0_setPrescaler(timer0_getRequiredPrescaler(Hz));
    timer0_setTop(fastPWM_Hz2Top(timer0_getPrescaler(), Hz));
  }
}

void timer0_setDuty(uint8_t _duty) {

  uint8_t dutyMapped;
  
  if (_duty >= 100)
    _duty = 99;

  timer0_duty = _duty;

  if (timer0_mode == PHASE_CORRECT_OCRA || timer0_mode == FASTPWM_OCRA || timer0_mode == CTC_OCRA){

	  if(timer0_getTop() % 2 != 0){
	  	dutyMapped = avrMap(timer0_duty, 0, 100, 0, timer0_getTop()-1);
	  }else{
		dutyMapped = avrMap(timer0_duty, 0, 100, 0, timer0_getTop());
	  }

	  if (dutyMapped <= 0) {
	    SET_TIMER0_OCRB(1);
	  }  
	  if (dutyMapped >= 255){
	    SET_TIMER0_OCRB(254);
	  }
	  else {
	    SET_TIMER0_OCRB(dutyMapped);
	  }
  }
}

uint16_t timer0_getPrescaler(void) { return timer0_prescaler; }

uint32_t timer0_getFrequency(void) {
  if (timer0_mode == PHASE_CORRECT_OCRA || timer0_mode == PHASE_CORRECT_TOP) {
    return phaseCorrectPWM_Top2Hz(timer0_getPrescaler(), timer0_getTop());
  }
  if (timer0_mode == FASTPWM_OCRA || timer0_mode == FASTPWM_TOP) {
    return fastPWM_Top2Hz(timer0_getPrescaler(), timer0_getTop());
            
  }
}

uint16_t timer0_getTop(void) {
  //  return ICR0; // Depending on PWM type used

  return timer0_top;
}

uint8_t timer0_getDuty(void) {
  //return avrMap(OCR0B, 0, timer0_getTop(), 0, 100);
  return timer0_duty;
}

void timer0_register_COMPA_callback(void (*func)(void)) {
  timer0_compa_handler = func;
}
void timer0_register_COMPB_callback(void (*func)(void)) {
  timer0_compb_handler = func;
}
void timer0_register_OVF_callback(void (*func)(void)) {
  timer0_ovf_handler = func;
}

ISR(TIMER0_COMPA_vect) { (*timer0_compa_handler)(); }

ISR(TIMER0_COMPB_vect) { (*timer0_compb_handler)(); }

ISR(TIMER0_OVF_vect) { (*timer0_ovf_handler)(); }
