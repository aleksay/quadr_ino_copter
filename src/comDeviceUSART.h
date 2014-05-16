/*
 Wrapper functions for Arduino library HardwareSerial 
 */

#include <stdio.h>
#include <Arduino.h>


/*
=====================================================
 
 		Clear bit, and set bit macros
 
 =====================================================
 */

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif 


int uart_putchar(char c, FILE *stream);
int uart_getchar(char c, FILE *stream);
int uart_available();
//void uart_init(void) __attribute__((naked)) __attribute__((section(".init7")));
void uart_init();
void stdio_init(void);
