#include <time.h>

volatile uint32_t milliseconds = 0;
volatile uint32_t seconds = 0;

void
timeClock_millisCallback ()
{

  milliseconds++;

  if (milliseconds >= 1000)
    {
      milliseconds = 0;
      seconds++;
    }
}

void
timeClock_init (void)
{

#ifdef _AVR_CLOCKTIMER_TIMER2_

  // timer2_init(CTC_OCRA, 128);
  // timer2_setTop(124);

  timer2_init (CTC_OCRA, 64);
  timer2_setTop (249);

  timer2_register_COMPA_callback (timeClock_millisCallback);
  timer2_start ();

#endif

#ifdef _AVR_CLOCKTIMER_TIMER0_

  timer0_init (CTC_OCRA, 64);
  timer0_setTop (249);
  // timer0_setDuty(50);
  timer0_register_COMPA_callback (timeClock_millisCallback);
  timer0_start ();

#endif
}

uint32_t
timeClock_getMillis ()
{
  return milliseconds;
}

uint32_t
timeClock_getSeconds ()
{
  return seconds;
}

void
timeClock_wait (uint32_t ms)
{

  if (ms > 1000)
    {

      uint32_t waitSeconds = ms / 1000;
      uint32_t waitMillis = ms % 1000;
      uint32_t startSeconds;
      // uint32_t startMillis;

      startSeconds = timeClock_getSeconds ();
      while (timeClock_getSeconds () <= startSeconds + waitSeconds)
        {
          // asm volatile("nop");
          timeClock_wait (1000);
        }

      timeClock_wait (waitMillis);

      // startMillis  = timeClock();
      // while( timeClock() <= startMillis + waitMillis ){
      //         asm volatile("nop");
      // }
    }
  else
    {
      while (timeClock_getMillis () % ms != 0)
        {
          asm volatile ("nop");
        }
    }
}
