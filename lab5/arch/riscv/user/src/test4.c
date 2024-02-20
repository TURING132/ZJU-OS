#include "stdio.h"
#include "getpid.h"
#include "mm.h"

int main() {
#ifndef BUDDY
  // test mapping without PTE_R/W
  printf("checking in test4\n");
  mmap(0x1000, 0x1000, PTE_V | PTE_U | PTE_W | PTE_X, 0, 0, 0);
  int* a = (int *)(0x1000);
  *a = 1;
  munmap(0x1000, 0x1000);
  mmap(0x1000, 0x1000, PTE_V | PTE_U | PTE_R | PTE_X, 0, 0, 0);

  if(*a != 1)
    printf("\033[32m[CHECK] page access control OK\033[0m\n\n");
  else
    printf("\033[31m[CHECK] page access control failed\033[0m\n\n");
#endif
  while (1)
    ;
  return 0;
}

void _start() {
  int ret = main();

}
