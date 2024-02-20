#include "stdio.h"
#include "getpid.h"
#include "mm.h"
#include "proc.h"

int main() {
  printf("[process print start]\n");
  mmap(0, 0x100000, PTE_V | PTE_U | PTE_R | PTE_X | PTE_W, 0, 0, 0);
  int *a = (int *)(0x0000);
  *a = 1;
  int ret = fork();
  if (ret == 0) {
    if (*a == 1) {
      printf("[child process] mypid: %d, a = %d\n", getpid(), *a);
    } else {
      printf("[child process] mypid: %d, a is not 1\n", getpid());
    }
  } else {
    printf("[father process] mypid: %d, child pid: %d\n", getpid(), ret);
    munmap(0, 0x1000);
    wait(ret);
  }
  return 0;
}