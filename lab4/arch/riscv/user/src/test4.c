#include "stdio.h"
#include "getpid.h"

int main() {
  while (1) {
    uint64_t sp = current_sp();
    long pid = getpid();
    printf("[TEST 4] pid: %ld, sp is %llx\n", pid, sp);
    for (unsigned int i = 0; i < 0xFFFFFFFF; i++)
      ;
  }
  return 0;
}

void _start() {
  int ret = main();

}
