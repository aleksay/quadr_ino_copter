#ifndef mosfetSeqController_h
#define mosfetSeqController_h

class mosfetSequenceController {

public:

  mosfetSequenceController(){}  
  
  int init();
	int getDirection();
  int setDirection();
  int automa_iterate();
  int setAutomaRate(int val);
  int getAutomaRate();
   
private:

};


#endif




