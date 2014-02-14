#include "timer.h"


class timer0: public timer
{
public:
	timer0(){

		pinMode(5,OUTPUT);
    pinMode(6,OUTPUT);
		frequency = 200;
		duty = 50;
		_dutyVal = 100;
		OCR0A = frequency;
		OCR0B = _dutyVal;

		TCCR0A =  _BV(COM0B1) | _BV(COM0B0) | _BV(WGM00) ;
    TCCR0B =  _BV(WGM02); 
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

		switch(_prescaler) {
			case 0:
				TCCR0B |= _BV(CS00);
				prescaler = 0;
		    return 0;
		  case 8:
				TCCR0B |= _BV(CS01);
				prescaler = 8;
		    return 0;
			case 64:
				TCCR0B |= _BV(CS01) | _BV(CS00);
				prescaler = 64;
				return 0;	
			case 256:
				TCCR0B |= _BV(CS02);
				prescaler = 256;
				return 0;
			case 1024:
				TCCR0B |= _BV(CS02) | _BV(CS00);
				prescaler = 1024;
				return 0;			
		}
		return 1;
	}

	int getPrescaler(){return prescaler;}

	int setFrequency(int val) {

		if (val == frequency) {
			return -1;
		}

		int zDuty = -10;

		if (frequency > val) {

			zDuty = setDuty(duty); 
			frequency = val;       
		  OCR0A = frequency;
		}

		if (frequency < val) {
			frequency = val;       
		  zDuty = setDuty(duty);
		  OCR0A = frequency;
		}
	
		return zDuty;
	}

	int setDuty(int val) {

		if (val < 0 || val >= 100)
			return -1;

		duty = val;
		_dutyVal = map(duty, 0, 100, 0, frequency);
		OCR0B = _dutyVal;
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
