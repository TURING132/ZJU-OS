#include "proc.h"
#include "syscall.h"

int fork() {
  struct ret_info ret = u_syscall(SYS_FORK, 0, 0, 0, 0, 0, 0);
  return ret.a0;
}

void wait(int pid) {
  u_syscall(SYS_WAIT, pid, 0, 0, 0, 0, 0);
}

void exit(int ret) {
  u_syscall(SYS_EXIT, ret, 0, 0, 0, 0, 0);
}

void exec(const char * path) {
  u_syscall(SYS_EXEC, (uint64_t)path, 0, 0, 0, 0, 0);
}