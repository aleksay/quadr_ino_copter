/*


*/

#ifndef BRUSHLESS_h
#define BRUSHLESS_h


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
