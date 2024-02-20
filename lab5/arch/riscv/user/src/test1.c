#include "stdio.h"
#include "getpid.h"
#include "mm.h"

extern long test(int t);

int main() {
#ifndef BUDDY
  // test with no error
  printf("checking in test1\n");
  int *a = (int *)mmap(0, 0x1000, PTE_V | PTE_U | PTE_R | PTE_W | PTE_X, 0, 0, 0);
  if(a == (int *)(0x0000)) {
    *a = 1;
    printf("\033[32m[CHECK] page store OK\033[0m\n\n");
  }
  else {
    printf("\033[31m[ERROR] page store failed\033[0m\n\n");
  }

  a = mmap(0x1000, 0x9000, PTE_V | PTE_U | PTE_R | PTE_W | PTE_X, 0, 0, 0);
  if(a != (int *)(0x1000)) {
    printf("\033[31m[ERROR] page load failed\033[0m\n\n");
  }
  else {
    *a = 1;
    int b = *a;
    if (b == 1)
      printf("\033[32m[CHECK] page load OK\033[0m\n\n");
    else
      printf("\033[31m[ERRORInvalid permission!] page load failed\033[0m\n\n");
  }
#endif
  while (1)
    ;
  return 0;
}

void _start() {
  int ret = main();

}
