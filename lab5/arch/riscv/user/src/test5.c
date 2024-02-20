#include "stdio.h"
#include "getpid.h"
#include "mm.h"

int main() {
#ifndef BUDDY
  // test with no error
  printf("checking in test5\n");
  mmap(0, 0x1000, PTE_V | PTE_U | PTE_W | PTE_R | PTE_X, 0, 0, 0);


  mmap(0x1000, 0x9000, PTE_V | PTE_U | PTE_R | PTE_W | PTE_X, 0, 0, 0);
  
  int ret = munmap(0, 0x2000);
  int ret2 = munmap(0x2000, 0x9000);

  if(ret == -1 && ret2 == -1) {
    printf("\033[32m[CHECK] unmap OK\033[0m\n\n");
  }
  else {
    printf("\033[31m[ERROR] unmap error\033[0m\n\n");
  }

#endif
  while (1)
    ;
  return 0;
}

void _start() {
  int ret = main();

}
