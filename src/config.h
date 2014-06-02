#ifndef CONFIG_H
#define CONFIG_H
/*
=====================================================
 		           Global configuration Section
  =====================================================
 */

#ifndef F_CPU
#pragma warn F_CPU unset, setting to 8 MHz
#define F_CPU 8000000UL  //Uncomment for arduino Fio
//#define F_CPU 16000000UL  //Uncomment for arduino duemilanove
#endif

#define BAUD 57600
/*
=====================================================
 		           PARAMETRI MOTORE
=====================================================
 */

#define DEFAULT_INITIAL_STATE 0
#define NUM_POLES 12

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
#define DEFAULT_T1_INIT_FREQUENCY 1 //244 //Hz-> TOP:65534
#define DEFAULT_T1_INIT_DUTY 1
#define DEFAULT_T1_INIT_PRESCALER 1

#endif // HEADER
