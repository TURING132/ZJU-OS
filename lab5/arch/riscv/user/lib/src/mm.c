#include "mm.h"

#include "syscall.h"

void *mmap (void *__addr, size_t __len, int __prot, 
                int __flags, int __fd, __off_t __offset) {
    void* ret;
    ret = u_syscall(SYS_MMAP, (uint64_t)__addr, (uint64_t)__len, (uint64_t)__prot, (uint64_t)__flags, (uint64_t)__fd, (uint64_t)__offset).a0;
    return ret;

}

int munmap (void *__addr, size_t __len) {
   int ret;
   ret = u_syscall(SYS_MUNMAP, (uint64_t)__addr, (uint64_t)__len, 0, 0, 0, 0).a0;
   return ret;
}