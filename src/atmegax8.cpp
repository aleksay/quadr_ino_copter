
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
