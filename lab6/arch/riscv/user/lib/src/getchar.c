#include "getchar.h"
#include "syscall.h"

int getchar() {
  struct ret_info ret = u_syscall(SYS_READ, 0, 0, 0, 0, 0, 0);
  return (int)ret.a0;
}