#include <test_avrTimer1.h>

#include <time.h>
#include <comunicator.h>



void handlerBlinker(){

PORTD ^= (1 << PD4);

}


void test_avrTimer1(){



DDRD |= (1 << PD4);



timeClock_init();


timer1_init(FASTPWM_OCRA,1);
timer1_setPin('B',CLEAR);
//timer1_setPin('B',TOGGLE);

timer1_register_COMPA_callback(handlerBlinker);
//timer1_register_OVF_callback(handlerBlinker);


timer1_setDuty(50);
timer1_setTop(1000);


//timer1_setFrequency(90000);

timer1_start();

comunicator pippo = comunicator();
log_info("starting timer1 test suite");

int i=100;
/*for (;i<5000;i++){

timer1_setTop(i);
timeClock_wait(100);
}*/
for (;i>86;i--){

timer1_setTop(i);
timeClock_wait(10);
log_info("top: %d",i);
}
timer1_setDuty(50);
timer1_setTop(1000);
timeClock_wait(1000);

timer1_setFrequency(90000);
timeClock_wait(2000);
timer1_setFrequency(900);

while(1){}

}




