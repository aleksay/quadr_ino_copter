#include "comDeviceUSART.h"
/*
 Wrapper functions for Arduino library HardwareSerial 
 */



int uart_putchar(char c, FILE *stream) {
    return Serial.write(c);
}

int uart_getchar(char c, FILE *stream) {
    return Serial.read();
}

int uart_available() {
    return Serial.available();
  }

void uart_init(void) {
    Serial.begin(9600);
}

//
//int uart_putchar(char c, FILE *stream) {
//    if (c == '\n') {
//        uart_putchar('\r', stream);
//    }
//    loop_until_bit_is_set(UCSR0A, UDRE0);
//    UDR0 = c;
//}
//
//int uart_getchar(char c, FILE *stream) {
//    loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
//    return UDR0;
//}
//
//
//#define F_CPU 16000000UL
//#define BAUD 9600
//
//#include <util/setbaud.h>
//
//void uart_init(void) {
//    UBRR0H = UBRRH_VALUE;
//    UBRR0L = UBRRL_VALUE;
//
//#if USE_2X
//    UCSR0A |= _BV(U2X0);
//#else
//    UCSR0A &= ~(_BV(U2X0));
//#endif
//
//    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
//    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
//}


// FROM BOOTLOADER
//#include <inttypes.h>
//#include <avr/io.h>
//#include <avr/pgmspace.h>
//#include <avr/eeprom.h>
//#include <avr/interrupt.h>
//#include <util/delay.h>
//
//void uart_init(void) {
//  UBRRH = (((F_CPU/BAUD_RATE)/16)-1)>>8; 	// set baud rate
//  UBRRL = (((F_CPU/BAUD_RATE)/16)-1);
//  UCSRB = (1<<RXEN)|(1<<TXEN);  // enable Rx & Tx
//  UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);  // config USART; 8N1
//}


void stdio_init(void) {
    static FILE fd_in = {0};
    static FILE fd_out = {0};

   // fill in the UART file descriptor with pointer to writer.
   fdev_setup_stream (&fd_out, uart_putchar, NULL, _FDEV_SETUP_WRITE);
   fdev_setup_stream (&fd_in, uart_getchar, NULL, _FDEV_SETUP_READ);

   // The uart is the standard output device STDOUT.
   stdout = &fd_out ;
   stderr = &fd_out ;
   stdin = &fd_in ;
}
