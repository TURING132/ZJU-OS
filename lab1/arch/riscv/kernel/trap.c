#ifndef PRINT_ONLY
#include "defs.h"

extern main(), puts(), put_num(), ticks;
extern void clock_set_next_event(void);

void handler_s(uint64_t cause) {
  if ((cause>>63)==1) {
    // supervisor timer interrupt
    if ((cause&0xF)==5) {
      put_num(ticks++);
      puts(" ticks\n");
      clock_set_next_event();
    }
  }
}
#endif