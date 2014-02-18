#ifndef COMDEVICE_h
#define COMDEVICE_h

class comDevice {

public:

	comDevice() {
	}
	virtual char read() 			= 0;
	virtual int write(char c) = 0;
	virtual int available()		= 0;
};

#endif

