#include"avrTime.h"


void avrClockInit(void) {
  MsTimer2::set(mstimer2_timeout, avrNop);
  MsTimer2::start();
}

unsigned long avrClock(void) {
  return MsTimer2::count;
}

void avrNop(void) {
}
