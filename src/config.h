/*
=====================================================
 		           Global configuration Section
  =====================================================
 */

//#define F_CPU 8000000UL  //Uncomment for arduino Fio
#define F_CPU 16000000UL  //Uncomment for arduino duemilanove

/*
=====================================================
 		           PARAMETRI MOTORE
=====================================================
 */
 
#define DEFAULT_INITIAL_STATE 0
#define NUM_STATES 6
#define NUM_POLES 14
 
 /*
=====================================================
 		           PARAMETRI Timer0
=====================================================
 */
#define DEFAULT_T0_INIT_FREQUENCY 200 //ancora in TOP
#define DEFAULT_T0_INIT_DUTY 1
#define DEFAULT_T0_INIT_PRESCALER 1

#define RAMP_FIN_DUTY_T0 90 


 /*
=====================================================
 		           PARAMETRI Timer1
=====================================================
 */
#define DEFAULT_T1_INIT_FREQUENCY 245 //244 //Hz-> TOP:65534
#define DEFAULT_T1_INIT_DUTY 1
#define DEFAULT_T1_INIT_PRESCALER 1

#define RAMP_END_FREQUENCY_T1 3600 
#define RAMP_GAIN_FREQUENCY_T1 250

