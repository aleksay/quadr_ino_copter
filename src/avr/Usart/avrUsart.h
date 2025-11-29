#ifndef AVRUSART_H
#define AVRUSART_H

#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

#define UART_RX_BUFFER_SIZE 64
#define UART_TX_BUFFER_SIZE 64

static volatile uint8_t rx_buffer[UART_RX_BUFFER_SIZE];
static volatile uint8_t tx_buffer[UART_TX_BUFFER_SIZE];

static volatile uint8_t isr_callCounter = 0;

static volatile uint8_t rx_head = 0;
static volatile uint8_t rx_tail = 0;
static volatile uint8_t tx_head = 0;
static volatile uint8_t tx_tail = 0;

// --------------------
// Funzioni statiche locali
// --------------------
static int uart_putchar(char c, FILE *stream);
static int uart_getchar(FILE *stream);

int uart_putchar(char c);
int uart_getchar(void);

void uart_flush_rx_buffer(void);

void uart_init(uint16_t baud = 9600);

/**
 * Stream globale associato alla UART,
 * utilizzabile con fprintf, fscanf, ecc.
 *
 * Esempio:
 *   fprintf(&uart_stream, "Hello!\n");
 *   int c = fgetc(&uart_stream);
 */
extern FILE uart_stream;

#endif
