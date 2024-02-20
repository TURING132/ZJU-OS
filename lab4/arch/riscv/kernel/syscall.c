#include "syscall.h"

#include "task_manager.h"
#include "stdio.h"
#include "defs.h"


struct ret_info syscall(uint64_t syscall_num, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5) {
    // TODO: implement syscall function
    struct ret_info ret;
    switch (syscall_num) {
    case SYS_WRITE:
    {   
        int fd = arg0,size = arg2;
        if(fd == 1){
            char * s = (char *)arg1;
            for(int i = 0; i < size; i++) {
                putchar(s[i]);
            }
        }
        ret.a0 = size;
        break;
    }
    case SYS_GETPID:
    {
        ret.a0 = getpid();
        break;
    }
    default:
        printf("Unknown syscall! syscall_num = %d\n", syscall_num);
        while(1);
        break;
    }
    return ret;
}