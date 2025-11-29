// #include <avrTimer0.h>
// #include <avrTimer1.h>

#include <brushless.h>
#include <comunicator.h>

void handlerA(uint8_t v) { // toggle pin 13
}
void handlerB(uint8_t v) { // toggle pin v
}

int main(void) {

  /*
  //case 't': log_info("%lums", timeClock());
  case 'f': pwmSetFrequency(command->value);
  case 'd': pwmSetDuty(command->value);
  case 'y': pwmStop();
  case 'k': pwmSetPrescaler(command->value);

  case 'a': setISRFrequency(command->value);
  case 'b': angSpeed();

  case 'R': wdt_sw_reset(); // broken for now ??
  case 's': starting = 1;
  case 'x': stopISR();
  case 'm': manualMode();
  case 'u': rampAutomaFrequencyA.end = command->value;
  case 'i': rampAutomaFrequencyA.gain = (command->value);
  case 'v': pins_setDriveDirection(command->value);

  //case 'r': log_info("freeRAM = %d", freeRam());
  //case 'o': log_info("GAIN:%d,  END:%d", rampAutomaFrequencyA.gain,
  rampAutomaFrequencyA.end);
  //case 'p':
        log_info("--QUERY--\nISRFrequency: %u,pwmFrequency: %u, pwmDuty: %d\n",
  getISRFrequency(), pwmGetFrequency(), pwmGetDuty() ); debug("OCR1A %u OCR1B %u
  OCR0A %u OCR0B %u", OCR1A, OCR1B, OCR0A, OCR0B);




  */

  // brushless myMotor;

  init();

  char commands[] = {
      // 'f',
      // 'd',
      // 'y',
      // 'k',
      // 'a',
      'b',
      // 'R',
      's',
      // 'x',
      'm' //,
          // 'u',
          // 'i',
          // 'v'
  };

  void (*handlers[])(uint8_t) = {
      // myMotor.pwmSetFrequency,
      // myMotor.pwmSetDuty,
      // myMotor.pwmStop,
      // myMotor.pwmSetPrescaler,
      // myMotor.setISRFrequency,
      angSpeed,        // X
                       // myMotor.wdt_sw_reset,
      setStartupState, // X
      // myMotor.stopISR,
      manualMode //, // X
                 // myMotor.rampAutomaFrequencyA.end,
                 // myMotor.rampAutomaFrequencyA.gain,
                 // myMotor.pins_setDriveDirection

  };

  comunicator myComunicator = comunicator(commands, handlers, sizeof(commands));

  while (1) {
    myComunicator.read();
  }
  /*
      int manualMode(int val);
     int angSpeed();
       int setStartupState(int state);

      int startupCallback();




          char commands[] =             {'a','b'};
          void (*handlers[])(uint8_t) = { pwmSetFrequency, handlerB };


          comunicator pippo =  comunicator(commands, handlers, 2);


          while(1){
                  pippo.read();
          }
  */
  return 0;
}
