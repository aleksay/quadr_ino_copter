#ifndef aSDFASDF
#define aSDFASDF

#include <stdio.h>
#include <stdint.h>
#include "atmegax8.h"


/*
 Wrapper functions for Arduino library HardwareSerial 
 */

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

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>

#define inputBufferLength 20


extern char inputBuffer[inputBufferLength];
extern char asciiMessage[inputBufferLength];
int uart_putchar(char c, FILE *stream);
int uart_getchar(char c, FILE *stream);
int uart_available(void);
void uart_flush(void);
extern bool haveAsciiMessage;
//void uart_init(void) __attribute__((naked)) __attribute__((section(".init7")));
void uart_init(void);
void stdio_init(void);


#endif
