#include <test_comunicator.h>

void handlerA(uint8_t v) { // toggle pin 13
  PORTB ^= (1 << PB4);
}
void handlerB(uint8_t v) { // toggle pin v

  PORTB ^= (1 << PB5); // Toggle pin 13
}

void test_comunicator() {

  DDRB |= (1 << PB5); // Set pin 13 as an output,
  DDRB |= (1 << PB4); // Set pin 13 as an output,

  char commands[] = {'a', 'b'};
  void (*handlers[])(uint8_t) = {handlerA, handlerB};

  comunicator pippo = comunicator(commands, handlers, 2);

  fprintf(stdout, "Comunicator ready, waiting for a b command...");
  log_info("start loop");
  while (1) {
    pippo.read();
    log_info("looping baby");
    //		_delay_ms (500);
  }
}
