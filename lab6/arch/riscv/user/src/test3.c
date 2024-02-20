#include "stdio.h"
#include "getpid.h"
#include "mm.h"

int main() {
  printf("[process malloc start]\n");
  mmap(0, 0x100000, PTE_V | PTE_U | PTE_R | PTE_X | PTE_W, 0, 0, 0);
  int *a = (int *)(0x0000);
  *a = 1;
  printf("a = %d\n", *a);
  munmap(0, 0x1000);
  *a = 1;
  return 0;
}