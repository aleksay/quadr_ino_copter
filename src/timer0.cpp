#include "timer.h"
#include "communicator.h"

class timer0: public timer {

public:
	timer0() {

		frequency = DEFAULT_T0_INIT_FREQUENCY;
		setDuty(DEFAULT_T0_INIT_DUTY);
		prescaler = DEFAULT_T0_INIT_PRESCALER;

		_timer0_fastPwm_ocr0atop_init();
	}

	void _timer0_fastPwm_ocr0atop_init() {
		//cli();
		SET_TIMER0_PINB;
		TIMER0_RESET
		;
		SET_TIMER0_FREQUENCY(frequency);
		SET_TIMER0_DUTY(_dutyVal);

		SET_TIMER0_PINOUT(B);
		SET_TIMER0_MODE_PHASE_CORRECT_OCR0A
		;
		SET_TIMER0_PINB_NOTINVERTING(0);
		//SET_TIMER0_INTERRUPT_OUTPUTCOMPARE_B;
		//SET_TIMER0_INTERRUPT_OVERFLOW;
		//sei();
	}

	int start() {
		setPrescaler(prescaler);
		return 0;
	}
	int start(int _prescaler) {
		setPrescaler(_prescaler);
		return 0;
	}

	int setPrescaler(int _prescaler) {

		switch (_prescaler) {

		case 1:
			SET_TIMER0_PRESCALER_1;
			prescaler = 1;
			debug(String(prescaler), 3)
			;
			return 0;
		case 8:
			SET_TIMER0_PRESCALER_8;
			prescaler = 8;
			debug(String(prescaler), 3)
			;
			return 0;
		case 64:
			SET_TIMER0_PRESCALER_64;
			prescaler = 64;
			debug(String(prescaler), 3)
			;
			return 0;
		case 256:
			SET_TIMER0_PRESCALER_256;
			prescaler = 256;
			debug(String(prescaler), 3)
			;
			return 0;
		case 1024:
			SET_TIMER0_PRESCALER_1024;
			prescaler = 1024;
			debug(String(prescaler), 3)
			;
			return 0;
		}
		return 1;
	}

	int getPrescaler() {
		return prescaler;
	}

	int setFrequency(unsigned int val) {
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

	unsigned int getFrequency() {
		return frequency;
	}
	int getDuty() {
		return duty;
	}

private:

	unsigned int frequency;
	int duty;
	unsigned int _dutyVal;
	int prescaler;

};

