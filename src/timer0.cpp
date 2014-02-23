#include "timer.h"

#define DEBUG 1
#define DEBUG_LEVEL 4 // change me to desired logging level

#ifdef DEBUG
#define debug(msg,prio) if(prio < DEBUG_LEVEL){													\
													Serial.print(String(__FILE__));								\
										    	Serial.print(":");														\
													Serial.print(String(__LINE__));								\
													Serial.print(":");														\
													Serial.print(String(__FUNCTION__));						\
													Serial.print(" - ");													\
													Serial.println(msg);													\
												}
#else
#define debug(msg)
#endif

class timer0: public timer
{

public:
	timer0(){

		frequency = T0_FREQUENCY;
		setDuty(T0_DUTY);
		prescaler = DEFAULT_T0_INITIAL_PRESCALER;
		
		OCR0A = frequency;
		OCR0B = _dutyVal;

		_timer0_fastPwm_ocr0atop_init();
//		TCCR0A =  _BV(COM0B1) | _BV(WGM00) ;
//		TCCR0B =  _BV(WGM02); 
	}

	void _timer0_fastPwm_ocr0atop_init(){
		//cli();
		SET_TIMER0_PINB;
		SET_TIMER0_FREQUENCY(frequency);
		SET_TIMER0_DUTY(_dutyVal);
		TIMER0_RESET;
		SET_TIMER0_PINOUT(B);  
		SET_TIMER0_MODE_PHASE_CORRECT_OCR0A;
		SET_TIMER0_PINB_NOTINVERTING(0);
		//SET_TIMER0_INTERRUPT_OUTPUTCOMPARE_B;
		//SET_TIMER0_INTERRUPT_OVERFLOW;
		//sei();

		debug(String(TCCR0A),3);
		debug(String(TCCR0B),3);
	}

  	int start() {

		setPrescaler(prescaler);
		return 0;
	}
	int start(int _prescaler) {
		setPrescaler(_prescaler);
		return 0;
	}
 
	int setPrescaler(int _prescaler){
		debug("",3);
		switch(_prescaler) {
			case 0:
				SET_TIMER0_PRESCALER_0;
				prescaler = 0;
				debug(String(prescaler),3);
		    return 0;
		  case 8:
				SET_TIMER0_PRESCALER_8;
				prescaler = 8;
				debug(String(prescaler),3);
		    return 0;
			case 64:
				SET_TIMER0_PRESCALER_64;
				prescaler = 64;
				debug(String(prescaler),3);
				return 0;	
			case 256:
				SET_TIMER0_PRESCALER_256;
				prescaler = 256;
				debug(String(prescaler),3);
				return 0;
			case 1024:
				SET_TIMER0_PRESCALER_1024;
				prescaler = 1024;
				debug(String(prescaler),3);
				return 0;			
		}
		return 1;
	}

	int getPrescaler(){return prescaler;}


	int setFrequency(int val) {
		if (val < 0 || val > 254)
			return -1;

		if (val == frequency) 
			return -1;
		

		int zDuty = -10;

		
		frequency = val;       
		
		SET_TIMER0_FREQUENCY(frequency);
		zDuty = setDuty(duty);
		
	
		return zDuty;
	}

	int setDuty(int val) {

		if (val < 0 || val >= 100)
			return -1;

		duty = val;
		_dutyVal = map(duty, 0, 100, 0, frequency);
		SET_TIMER0_DUTY(_dutyVal);
		return 0;
	}

	int getFrequency() {
		return frequency;
	}
	int getDuty() {
		return duty;
	}



private:

 int frequency;
	int duty;
	int _dutyVal;
	int prescaler;

};
