#ifndef avrtime_h
#define avrtime_h

#include <MsTimer2.h>

#define mstimer2_timeout 4294967295 // 32bit

void avrClockInit(void);
unsigned long avrClock(void);
void avrNop(void);

#endif // header include
