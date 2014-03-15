#include <Arduino.h>
#include "comDevice.h"

/*
 Wrapper class for Arduino library HardwareSerial

 */
class usart: public comDevice {
public:

	usart(int baud) {
		Serial.begin(baud);
	}
	char read() {
		return Serial.read();
	}
	int write(char c) {
		return Serial.write(c);
	}
	int available() {
		return Serial.available();
	}

};
