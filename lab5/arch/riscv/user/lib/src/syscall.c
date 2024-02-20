#include "syscall.h"


struct ret_info u_syscall(uint64_t syscall_num, uint64_t arg0, uint64_t arg1, uint64_t arg2, \
                uint64_t arg3, uint64_t arg4, uint64_t arg5){
    struct ret_info ret;
    // 完成系统调用，将syscall_num放在a7中，将参数放在a0-a5中，触发ecall，将返回值放在ret中
    asm volatile ("mv a7, %2;"
                    "mv a0, %3;"
                    "mv a1, %4;"
                    "mv a2, %5;"
                    "mv a3, %6;"
                    "mv a4, %7;"
                    "mv a5, %8;"
                    "ecall;"
                    "mv %0, a0;"
                    "mv %1, a1;"
                    : "=r"(ret.a0), "=r"(ret.a1)
                    : "r"(syscall_num), "r"(arg0), "r"(arg1), "r"(arg2), "r"(arg3), "r"(arg4), "r"(arg5)
                    : "a0", "a1", "a2", "a3", "a4", "a5", "a7");
                  
    return ret;
}
