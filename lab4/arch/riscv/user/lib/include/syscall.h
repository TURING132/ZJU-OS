#pragma once

#define SYS_WRITE 64
#define SYS_GETPID 172

#include "types.h"

struct ret_info {
    uint64_t a0;
    uint64_t a1;
};

struct ret_info u_syscall(uint64_t syscall_num, uint64_t arg0, uint64_t arg1, uint64_t arg2, \
                uint64_t arg3, uint64_t arg4, uint64_t arg5);