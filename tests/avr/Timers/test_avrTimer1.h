#ifndef TESTAVRtest_timer1_H
#define TESTAVRtest_timer1_H

#include <avrTimer1.h>

uint8_t test_timer1_init();
uint8_t test_timer1_fastPwm_icr1top_init();
uint8_t test_timer1_fastPwm_ocr1atop_init();
uint8_t test_timer1_start(uint16_t Hz);
uint8_t test_timer1_stop();
uint8_t test_timer1_getPrescalerMinHz(void);
uint8_t test_timer1_getPrescalerRequired(uint16_t Hz);
uint8_t test_timer1_setPrescaler(uint16_t _prescaler);
uint8_t test_timer1_setFrequency(uint16_t Hz);
uint8_t test_timer1_setTop(uint16_t top);
uint8_t test_timer1_setDuty(uint8_t duty);
uint8_t test_timer1_getPrescaler(void);
uint8_t test_timer1_getFrequency(void);
uint8_t test_timer1_getTop(void);
uint8_t test_timer1_getDuty(void);
uint8_t test_timer1_ovf_handler(void);

#endif
