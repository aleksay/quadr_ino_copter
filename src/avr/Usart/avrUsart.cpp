#include <avrUsart.h>

// --------------------
// Stream stdio
// --------------------
FILE uart_stream
    = FDEV_SETUP_STREAM (uart_putchar, uart_getchar, _FDEV_SETUP_RW);

// --------------------
// Inizializzazione UART
// --------------------
void
uart_init (uint16_t baud)
{
  uint16_t ubrr_value = (F_CPU / (16UL * baud)) - 1;

  UBRR0H = (uint8_t)(ubrr_value >> 8);
  UBRR0L = (uint8_t)(ubrr_value);

  // ABILITA SOLO RX E TX INIZIALMENTE, RXCIE0 sarà abilitato dopo
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

  // Pulisce eventuali flag pendenti
  uint8_t dummy = UDR0;
  (void)dummy;

  // Ora abilita l'interrupt di ricezione
  UCSR0B |= (1 << RXCIE0);

  sei ();
}

static int
uart_putchar (char c, FILE *stream)
{
  return uart_putchar (c);
}
static int
uart_getchar (FILE *stream)
{
  return uart_getchar ();
}

// --------------------
// ISR Ricezione
// --------------------
ISR (USART_RX_vect)
{

  uint8_t data = UDR0; // Legge il dato - questo azzera RXC0
  uint8_t next = (rx_head + 1) % UART_RX_BUFFER_SIZE;

  if (next != rx_tail)
    {
      rx_buffer[rx_head] = data;
      rx_head = next;
    }
}

// --------------------
// ISR Trasmissione
// --------------------

ISR (USART_UDRE_vect)
{
  if (tx_head != tx_tail)
    {
      UDR0 = tx_buffer[tx_tail];
      tx_tail = (tx_tail + 1) % UART_TX_BUFFER_SIZE;
    }
  else
    {
      UCSR0B &= ~(1 << UDRIE0);
    }
}

// --------------------
// Scrittura carattere (fprintf, printf, ecc.)
// --------------------
int
uart_putchar (char c)
{
  if (c == '\n')
    {
      uart_putchar ('\r');
    }

  uint8_t next = (tx_head + 1) % UART_TX_BUFFER_SIZE;

  // Attende spazio disponibile
  while (next == tx_tail)
    {
      // Yield per evitare busy waiting
      asm volatile ("nop");
    }

  uint8_t oldSREG = SREG;
  cli ();
  tx_buffer[tx_head] = (uint8_t)c;
  tx_head = next;
  UCSR0B |= (1 << UDRIE0);
  SREG = oldSREG;

  return 0;
}

int
uart_getchar ()
{
  // Attende finché non arriva un carattere
  // log_info(" Attendo finche non arriva un carattere ");
  while (rx_head == rx_tail)
    {
      // Yield per evitare busy waiting
      asm volatile ("nop");
    }

  uint8_t oldSREG = SREG;
  cli ();
  uint8_t data = rx_buffer[rx_tail];
  rx_tail = (rx_tail + 1) % UART_RX_BUFFER_SIZE;
  SREG = oldSREG;
  // log_info("sbloccato, head: %d, tail %d", rx_head,rx_tail);
  uart_putchar (data);
  return data;
}

void
uart_flush_rx_buffer ()
{

  rx_head = 0;
  rx_tail = 0;
}
