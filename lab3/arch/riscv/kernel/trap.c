#include "defs.h"
#include "sched.h"
#include "stdio.h"
#include "test.h"

void handler_s(uint64_t cause, uint64_t epc) {
  // interrupt
  if (cause >> 63 == 1) {
    // supervisor timer interrupt
    if (cause == 0x8000000000000005) {
      asm volatile("ecall");
      ticks++;
      if (ticks % 10 == 0) {
        do_timer();
      }
    }
  }
  // exception
  else if (cause >> 63 == 0) {
    // instruction page fault
    if (cause == 0xc) {
      printf("Instruction page fault! epc = 0x%016lx\n", epc);
    }
    else if (cause == 0xd) {
      printf("Load page fault! epc = 0x%016lx\n", epc);
    }
    else if (cause == 0xf) {
      printf("Store/AMO page fault! epc = 0x%016lx\n", epc);
    }
    else {
      printf("Unknown exception! epc = 0x%016lx\n", epc);
    }
    while (1);
  }
  return;
}
