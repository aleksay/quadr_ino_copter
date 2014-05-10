
#include "atmegax8.h"


int freeRam ()
{
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}


// Software reset
void wdt_init(void)
{
  MCUSR = 0;
  wdt_disable();
  return;
}


// Software reset
void printAndClearResetSource(void)
{

  if (bit_get(MCUSR, BIT(0)))
  {
    //log_info("Power-on reset");
  }
  if (bit_get(MCUSR, BIT(1)))
  {
    //log_info("External reset");
  }
  if (bit_get(MCUSR, BIT(2)))
  {
    //log_info("Brown-out reset");
  }
  if (bit_get(MCUSR, BIT(3)))
  {
    //log_info("WDog system reset");
  }

  MCUSR = 0; // clear reset flag for next reset
  return;
}
