#include <test_avrUsart.h>

uint8_t test_uart_putchar(char c, FILE *stream) { return 0; }
uint8_t test_uart_getchar(char c, FILE *stream) { return 0; }

uint8_t test_uart_init(void) {

  // chiamare la uart_init();
  // valutare la forma dei registri cjhe deve corrispondere al codice

  DDRB |= (1 << PB5);

  uart_init(9600);

  /*

  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);;
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);



  */

  _delay_ms(500);

  uart_putchar('c');
  uart_putchar('i');
  uart_putchar('a');
  uart_putchar('o');
  uart_putchar('\n');

  char c;

  PORTB ^= (1 << PB5);

  while (1) {

    c = uart_getchar();

    uart_putchar(c);
    uart_putchar('\n');
    PORTB ^= (1 << PB5);
    _delay_ms(500);
  }

  return 0;
}
