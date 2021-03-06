#ifndef AVRUTILS
#define AVRUTILS 


#include <avr/wdt.h>
#include <avr/interrupt.h> 
#include "comLogger.h"
#include "config.h"


/* 
 
 =====================================================
 
 Bit manipulation macros
 
 =====================================================
 
 To set a bit: 
 bit_set(foo, 0x01); 
 
 To set bit number 5: 
 bit_set(foo, BIT(5)); 
 
 To clear bit number 6 with a bit mask: 
 bit_clear(foo, 0x40); 
 
 To flip bit number 0: 
 bit_flip(foo, BIT(0)); 
 
 To check bit number 3: 
 if(bit_get(foo, BIT(3))) 
 
 To set or clear a bit based on bit number 4: 
 if(bit_get(foo, BIT(4))) 
 { 
 bit_set(bar, BIT(0)); 
 } 
 else 
 { 
 bit_clear(bar, BIT(0)); 
 } 
 */

#define bit_get(p,m) ((p) & (m))
#define bit_set(p,m) ((p) |= (m)) 
#define bit_clear(p,m) ((p) &= ~(m)) 
#define bit_flip(p,m) ((p) ^= (m)) 
#define bit_write(c,p,m) (c ? bit_set(p,m) : bit_clear(p,m)) 
#define BIT(x) (0x01 << (x)) 








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


//! Boolean FALSE value.
#define FALSE     0

//! Boolean TRUE expression. Can be used both for test and assignment.
#define TRUE      (!FALSE)



int freeRam () ;


void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));

// Software reset 
void wdt_sw_reset(void);

// get reset cause
void printAndClearResetSource(void);

long avrMap(long x, long in_min, long in_max, long out_min, long out_max);

#endif


