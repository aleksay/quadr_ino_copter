#include "comDeviceUSART.h"
/*
 Wrapper functions for Arduino library HardwareSerial
 */



//int uart_putchar(char c, FILE *stream) {
//    return Serial.write(c);
//}
//
//int uart_getchar(char c, FILE *stream) {
//    return Serial.read();
//}

int uart_available() {
  return Serial.available();
}

//void uart_init(void) {
//    Serial.begin(9600);
//}


int uart_putchar(char c, FILE *stream) {
  if (c == '\n') {
    uart_putchar('\r', stream);
  }
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = c;
}

int uart_getchar(char c, FILE *stream) {
  loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
  return UDR0;
}



//#define BAUD 9600
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
//
//}



#define SERIAL_8N1 0x06


void uart_init(void)
{
  uint16_t baud = 9600;
  byte config = SERIAL_8N1;
  
  // Try u2x mode first
  uint16_t baud_setting = (F_CPU / 4 / baud - 1) / 2;
  UCSR0A = 1 << U2X0;

  // hardcoded exception for 57600 for compatibility with the bootloader
  // shipped with the Duemilanove and previous boards and the firmware
  // on the 8U2 on the Uno and Mega 2560. Also, The baud_setting cannot
  // be > 4095, so switch back to non-u2x mode if the baud rate is too
  // low.
  if (((F_CPU == 16000000UL) && (baud == 57600)) || (baud_setting > 4095))
  {
    UCSR0A = 0;
    baud_setting = (F_CPU / 8 / baud - 1) / 2;
  }

  // assign the baud_setting, a.k.a. ubbr (USART Baud Rate Register)
  UBRR0H = baud_setting >> 8;
  UBRR0L = baud_setting;


  //set the data bits, parity, and stop bits
#if defined(__AVR_ATmega8__)
  config |= 0x80; // select UCSRC register (shared with UBRRH)
#endif

  sbi(config, RXEN0);
  sbi(config, TXEN0);
  sbi(config, RXCIE0);
  cbi(config, UDRIE0);


//  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
//  UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */

}




//
//// FROM BOOTLOADER
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
