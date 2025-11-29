#include <avrTimer2.h>

void (*timer2_ovf_handler)();
void (*timer2_compa_handler)();
void (*timer2_compb_handler)();

uint16_t timer2_allowedPrescalers[] = { 1, 8, 32, 64, 128, 256, 1024};
uint8_t timer2_allowedPrescalersLenght =
    sizeof(timer2_allowedPrescalers) / sizeof(timer2_allowedPrescalers[0]);

// Var declarations
timer_mode timer2_mode = NORMAL;
uint16_t timer2_prescaler = 0;
volatile uint8_t timer2_bottom = 0;
volatile uint16_t timer2_top = 0;
uint8_t timer2_duty = 0;
uint8_t timer2_running = 0; // 0 FALSE; 1 TRUE

uint8_t timer2_isRunning() { return timer2_running; }

void timer2_init() {
  timer2_stop();
  timer2_mode_normal_init();
  timer2_setPrescaler((uint16_t)128);
  timer2_setBottom(256 - (int)((float)F_CPU * 0.001 / timer2_prescaler));
}

void timer2_init(timer_mode _mode, uint16_t _prescaler) {

  timer2_stop();

  if (_mode == NORMAL) {
    timer2_mode = _mode;
    timer2_mode_normal_init();
    timer2_setPrescaler(_prescaler);
    timer2_setBottom(256 - (int)((float)F_CPU * 0.001 / timer2_prescaler));
    // da usare con ISR(TIMER2_OVF_vect)
  }
  if (_mode == PHASE_CORRECT_OCRA) {

    timer2_mode = _mode;
    timer2_mode_phasecorrect_ocra_init();

    timer2_setPrescaler(_prescaler);
    // timer2_setFrequency(0); //also sets duty
  }
  if (_mode == CTC_OCRA) {
    timer2_mode = _mode;
    timer2_mode_ctc_ocra_init();
    timer2_setPrescaler(_prescaler);
    // da usare con timer2_register_COMPA_callback
  }
  if (_mode == FASTPWM_OCRA){
    timer2_mode = _mode;
    timer2_mode_fastpwm_ocra_init();
    // set pin B
    timer2_setPrescaler(_prescaler);
  
  }
}

void timer2_mode_normal_init() {

  UNSET_TIMER2_PINA;
  UNSET_TIMER2_PINB;

  TIMER2_RESET;

  SET_TIMER2_MODE_NORMAL;
  // SET_TIMER2_CLOCK_INTERNAL;
  // UNSET_TIMER2_INTERRUPT_OUTPUTCOMPARE_A;
}
void timer2_mode_ctc_ocra_init() {

  UNSET_TIMER2_PINA;
  UNSET_TIMER2_PINB;

  TIMER2_RESET;
  SET_TIMER2_CLOCK_INTERNAL;
  SET_TIMER2_MODE_CTC_OCRA;
}
void timer2_mode_phasecorrect_ocra_init() {

  UNSET_TIMER2_PINA;
  UNSET_TIMER2_PINB;

  TIMER2_RESET;

  SET_TIMER2_MODE_PHASE_CORRECT_OCRA;
}

void timer2_mode_phasecorrect_top_init(){}
void timer2_mode_fastpwm_top_init(){}
void timer2_mode_fastpwm_ocra_init(){

  UNSET_TIMER2_PINA;
  UNSET_TIMER2_PINB;

  TIMER2_RESET;

//  SET_TIMER2_PINB;
 
//  SET_TIMER2_PINOUT(B);
//  SET_TIMER2_PINB_NOTINVERTING(0);

  SET_TIMER2_MODE_FASTPWM_OCRA;

}

void timer2_clearPin(char _pin) {
  if (_pin == 'A') {
    SET_TIMER2_PINA_DISABLE;
    SET_TIMER2_PINA_CLEAR;
    UNSET_TIMER2_PINA;
  }
  if (_pin == 'B') {
    SET_TIMER2_PINB_DISABLE;
    SET_TIMER2_PINB_CLEAR;
    UNSET_TIMER2_PINB;
  }
}

void timer2_setPin(char _pin, timer_pin_mode _mode) {

  if (_pin == 'A') {
    SET_TIMER2_PINA;
    SET_TIMER2_PINA_CLEAR;

    if (_mode == TOGGLE) {
      SET_TIMER2_PINA_TOGGLE_ONCOMPARE;
    }
    if (_mode == CLEAR) {
      SET_TIMER2_PINA_CLEAR_ONCOMPARE;
    }
    if (_mode == SET) {
      SET_TIMER2_PINA_SET_ONCOMPARE;
    }
  }

  if (_pin == 'B') {
    SET_TIMER2_PINB;
    SET_TIMER2_PINB_CLEAR;

    if (_mode == TOGGLE) {
      SET_TIMER2_PINB_TOGGLE_ONCOMPARE;
    }
    if (_mode == CLEAR) {
      SET_TIMER2_PINB_CLEAR_ONCOMPARE;
    }
    if (_mode == SET) {
      SET_TIMER2_PINB_SET_ONCOMPARE;
    }
  }
}



void _timer2_setPrescaler(uint16_t _prescaler) {

  switch (_prescaler) {

  case 0:
    SET_TIMER2_PRESCALER_0;
    return;
  case 1:
    SET_TIMER2_PRESCALER_1;
    return;
  case 8:
    SET_TIMER2_PRESCALER_8;
    return;
  case 32:
    SET_TIMER2_PRESCALER_32;
    return;
  case 64:
    SET_TIMER2_PRESCALER_64;
    return;
  case 128:
    SET_TIMER2_PRESCALER_128;
    return;
  case 256:
    SET_TIMER2_PRESCALER_256;
    return;
  case 1024:
    SET_TIMER2_PRESCALER_1024;
    return;
  default:
    return;
  }
}

void timer2_setPrescaler(uint16_t _prescaler) {

  int i;
  for (i = 0; i < timer2_allowedPrescalersLenght; i++) {
    if (_prescaler == timer2_allowedPrescalers[i]) {
      timer2_prescaler = _prescaler;
      if (timer2_isRunning()) {
        _timer2_setPrescaler(_prescaler);
      }
    }
  }
}

uint16_t timer2_getPrevPrescaler() {

  int i;
  for (i = 0; i < timer2_allowedPrescalersLenght; i++) {
    if (timer2_prescaler == timer2_allowedPrescalers[i]) {
      return timer2_allowedPrescalers[(i - 1) % timer2_allowedPrescalersLenght];
    }
  }
}

uint16_t timer2_getNextPrescaler() {

  int i;
  for (i = 0; i < timer2_allowedPrescalersLenght; i++) {
    if (timer2_prescaler == timer2_allowedPrescalers[i]) {
      return timer2_allowedPrescalers[(i + 1) % timer2_allowedPrescalersLenght];
    }
  }
}

void timer2_increasePrescaler() {
  timer2_setPrescaler(timer2_getNextPrescaler());
}
void timer2_decreasePrescaler() {
  timer2_setPrescaler(timer2_getPrevPrescaler());
}

uint16_t timer2_getRequiredPrescaler(uint32_t Hz) {

  int i;
  for (i = 0; i < timer2_allowedPrescalersLenght; i++) {
    if (Hz >= prescalerMinHz(timer2_mode, timer2_allowedPrescalers[i], 8)) {
      return timer2_allowedPrescalers[i];
    }
  }
}

void timer2_resetCounter() {
  if (timer2_mode == NORMAL) {
    TCNT2 = timer2_bottom;
  }
  if (timer2_mode == CTC_OCRA || timer2_mode == FASTPWM_OCRA || timer2_mode == PHASE_CORRECT_OCRA) {
    TCNT2 = 0;
  }
}

void timer2_start() { timer2_start(timer2_prescaler); }

void timer2_start(uint16_t _prescaler) {
  if (timer2_mode == NORMAL) {
    timer2_resetCounter();
    SET_TIMER2_INTERRUPT_OVERFLOW;
  }
  if (timer2_mode == CTC_OCRA) {
    timer2_resetCounter();
    SET_TIMER2_INTERRUPT_OUTPUTCOMPARE_A;
  }
  if (timer2_mode == PHASE_CORRECT_OCRA) {
  }
  if (timer2_mode == FASTPWM_OCRA) {
  }
  
  timer2_running = 1;
  timer2_setPrescaler(_prescaler);
}

void timer2_stop() {
  if (timer2_mode == NORMAL) {
    UNSET_TIMER2_INTERRUPT_OVERFLOW;    
  }
  if (timer2_mode == CTC_OCRA) {
    UNSET_TIMER2_INTERRUPT_OUTPUTCOMPARE_A;    
  }
  if (timer2_mode == PHASE_CORRECT_OCRA) {    
  }
  if (timer2_mode == FASTPWM_OCRA) {
  }
  _timer2_setPrescaler(0);
  timer2_running = 0;
}

void timer2_setTop(uint16_t _top) {

 // uint8_t old_duty = timer2_getDuty();

  if (_top <= 2)
    timer2_top = 2;
  else
    timer2_top = _top;

  // IF mode: normal -> timer2_top represent the maximum number of
  // TIMER2_OVF_vect before executing timer2_ovf_handler
  if (timer2_mode == NORMAL) {
  }
  if (timer2_mode == PHASE_CORRECT_OCRA || timer2_mode == CTC_OCRA || timer2_mode == FASTPWM_OCRA) {

    SET_TIMER2_OCRA(timer2_top);
    timer2_setDuty(timer2_duty);
  }

  
}

void timer2_setBottom(uint8_t _bottom) { timer2_bottom = _bottom; }

void timer2_setFrequency(uint32_t Hz) {

  // 15700 is minimum frequency for prescaler 1 at 8 MHz.
  //  if (Hz <= 15700 || Hz > 65000) {
  //    return;
  //  }

  // convert Hz to register value TOP
  //  timer2_setTop( phaseCorrectPWM_Hz2Top(timer2_getPrescaler(), Hz) );
  //  timer2_setDuty(timer2_getDuty());

  if (timer2_mode == PHASE_CORRECT_OCRA || timer2_mode == PHASE_CORRECT_TOP) {

    timer2_setPrescaler(timer2_getRequiredPrescaler(Hz));
    timer2_setTop(phaseCorrectPWM_Hz2Top(timer2_getPrescaler(), Hz));
  }
  if (timer2_mode == FASTPWM_OCRA || timer2_mode == FASTPWM_TOP) {

    timer2_setPrescaler(timer2_getRequiredPrescaler(Hz));
    timer2_setTop(fastPWM_Hz2Top(timer2_getPrescaler(), Hz));
  }
}

void timer2_setDuty(uint8_t _duty) {

  uint8_t dutyMapped;
  
  if (_duty >= 100)
    _duty = 99;

  timer2_duty = _duty;

  if (timer2_mode == PHASE_CORRECT_OCRA || timer2_mode == FASTPWM_OCRA || timer2_mode == CTC_OCRA){

	  if(timer2_getTop() % 2 != 0){
	  	dutyMapped = avrMap(timer2_duty, 0, 100, 0, timer2_getTop()-1);
	  }else{
		dutyMapped = avrMap(timer2_duty, 0, 100, 0, timer2_getTop());
	  }

	  if (dutyMapped <= 0) {
	    SET_TIMER2_OCRB(1);
	  }  
	  if (dutyMapped >= 255){
	    SET_TIMER2_OCRB(254);
	  }
	  else {
	    SET_TIMER2_OCRB(dutyMapped);
	  }
  }
}

uint16_t timer2_getPrescaler(void) { return timer2_prescaler; }

uint32_t timer2_getFrequency(void) {
  if (timer2_mode == PHASE_CORRECT_OCRA || timer2_mode == PHASE_CORRECT_TOP) {
    return phaseCorrectPWM_Top2Hz(timer2_getPrescaler(), timer2_getTop());
  }
  if (timer2_mode == FASTPWM_OCRA || timer2_mode == FASTPWM_TOP) {
    return fastPWM_Top2Hz(timer2_getPrescaler(), timer2_getTop());
            
  }
}

uint16_t timer2_getTop(void) {
  //  return ICR0; // Depending on PWM type used

  return timer2_top;
}

uint8_t timer2_getDuty(void) {
  //return avrMap(OCR2B, 0, timer2_getTop(), 0, 100);
  return timer2_duty;
}

void timer2_register_COMPA_callback(void (*func)(void)) {
  timer2_compa_handler = func;
}
void timer2_register_COMPB_callback(void (*func)(void)) {
  timer2_compb_handler = func;
}
void timer2_register_OVF_callback(void (*func)(void)) {
  timer2_ovf_handler = func;
}

ISR(TIMER2_COMPA_vect) { (*timer2_compa_handler)(); }

ISR(TIMER2_COMPB_vect) { (*timer2_compb_handler)(); }

ISR(TIMER2_OVF_vect) { (*timer2_ovf_handler)(); }
