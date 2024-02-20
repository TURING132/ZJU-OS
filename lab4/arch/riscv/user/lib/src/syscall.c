#include "syscall.h"


struct ret_info u_syscall(uint64_t syscall_num, uint64_t arg0, uint64_t arg1, uint64_t arg2, \
                uint64_t arg3, uint64_t arg4, uint64_t arg5){
    struct ret_info ret;
    // TODO: 完成系统调用，将syscall_num放在a7中，将参数放在a0-a5中，触发ecall，将返回值放在ret中
    uint64_t temp_error , temp_value;
    __asm__ volatile(
        "mv a7, %[syscall_num]\n"
        "mv a5, %[arg5]\n"
        "mv a4, %[arg4]\n"
        "mv a3, %[arg3]\n"
        "mv a2, %[arg2]\n"
        "mv a1, %[arg1]\n"
        "mv a0, %[arg0]\n"
        
        "ecall\n"
        "mv %[temp_error], a0\n"
        "mv %[temp_value], a1\n"

        // 输出操作数
        : [temp_error] "=r" (temp_error), [temp_value] "=r" (temp_value)
        // 输入操作数
        : [syscall_num] "r" (syscall_num),[arg0] "r"(arg0), [arg1] "r"(arg1), [arg2] "r"(arg2), [arg3] "r"(arg3), [arg4] "r"(arg4), [arg5] "r"(arg5)
        //可能影响的寄存器
        : "memory","a0","a1","a2","a3","a4","a5","a7"
    );
    
    ret.a0 = temp_error;
    ret.a1 = temp_value;
                  
    return ret;
}