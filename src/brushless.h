/*


*/

#ifndef BRUSHLESS_h
#define BRUSHLESS_h


typedef struct _startup_data{
   int start;
   int end;
   float decrement;
   int currentValue;
   float resto;
} *startupData;


class brushless {

  public:

    brushless();
    int setFrequency(int val);
    int setRefreshRate(int val);
    int setDuty(int val);
    int getFrequency();
    int getRefreshRate();
    int getDuty();
    int eventHandler();
    int startup();
    void startupcalc(startupData valueData,int slow);
    //void iterate();

  private:

    volatile unsigned int cpmCounter;
    volatile int          stato;
    volatile unsigned int frequency;//holds register value
    volatile unsigned int refreshRate;
    volatile unsigned int duty;//holds register value

    int timer1_init();
};

#endif
