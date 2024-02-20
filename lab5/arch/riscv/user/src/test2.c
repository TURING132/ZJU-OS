#include "stdio.h"
#include "getpid.h"
#include "mm.h"

int main() {
#ifndef BUDDY
  // test with no error
  printf("checking in test2\n");
  mmap(0, 0x1000, PTE_V | PTE_U | PTE_R | PTE_X, 0, 0, 0);
  int *a = (int *)(0x0000);
  *a = 1;

  mmap(0x1000, 0x9000, PTE_V | PTE_U | PTE_R | PTE_W | PTE_X, 0, 0, 0);
  a = (int *)(0x8000);
  *a = 1;
  int b = *a;
  
  int ret = munmap(0, 0x1000);
  int ret2 = munmap(0x1000, 0x9000);

  if(ret == -1 || ret2 == -1) {
    printf("\033[31m[ERROR] unmap failed\033[0m\n\n");
  }
  else {  
    a = (int *)(0x0000);
    int c = 0;
    c = *a;
    a = (int *)(0x8000);
    b = 0;
    b = *a;
    if (c == 1 || b == 1)
      printf("\033[31m[CHECK] page unmap failed\033[0m\n\n");
    else
      printf("\033[32m[CHECK] unmap OK\033[0m\n\n");
  }
#endif

  while (1)
    ;
  return 0;
}

void _start() {
  int ret = main();

}

