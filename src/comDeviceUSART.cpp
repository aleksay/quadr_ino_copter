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
    int baud = 9600;
    Serial.begin(baud);
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
