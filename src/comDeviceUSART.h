/*
 Wrapper functions for Arduino library HardwareSerial 
 */

#include <stdio.h>
#include <Arduino.h>




int uart_putchar(char c, FILE *stream);
int uart_getchar(char c, FILE *stream);
int uart_available();
void uart_init(void);
