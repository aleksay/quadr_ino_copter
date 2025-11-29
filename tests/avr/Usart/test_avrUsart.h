#ifndef TESTAVRUSART_H
#define TESTAVRUSART_H

#include <avr/io.h>
#include <logger.h>
#include <util/delay.h>

#include <avrUsart.h>

uint8_t test_uart_putchar(char c, FILE *stream);
uint8_t test_uart_getchar(char c, FILE *stream);
uint8_t test_uart_init(void);

#endif
