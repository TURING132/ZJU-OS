#include "syscall.h"

#include "task_manager.h"
#include "stdio.h"
#include "defs.h"
#include "slub.h"
#include "mm.h"


struct ret_info syscall(uint64_t syscall_num, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5) {
    struct ret_info ret;
    switch (syscall_num) {
    case SYS_GETPID: {
        ret.a0 = getpid();
        break;
    }
    case SYS_WRITE: {
        int fd = arg0;
        char* buffer = (char*)arg1;
        int size = arg2;
        if(fd == 1) {
            for(int i = 0; i < size; i++) {
                putchar(buffer[i]);
            }
        }
        ret.a0 = size;
        break;
    }
    case SYS_MMAP: {
        // TODO: implement mmap
        // 1. create a new vma struct (kmalloc), if kmalloc failed, return -1
        // 2. initialize the vma struct
        // 2.1 set the vm_start and vm_end according to arg0 and arg1
        // 2.2 set the vm_flags to arg2
        // 2.3 set the mapped flag to 0
        // 3. add the vma struct to the mm_struct's vma list
        // return the vm_start
        struct vm_area_struct* vma = kmalloc(sizeof(struct vm_area_struct));
        vma->vm_start = arg0;
        vma->vm_end = arg0 + arg1;
        vma->vm_flags = arg2;
        vma->mapped = 0;
        list_add(&(vma->vm_list),&(current->mm.vm->vm_list));
        ret.a0 = vma->vm_start;
        break;

    }
    case SYS_MUNMAP: {
        // TODO: implement munmap
        // note: you can iterate the vm_list by list_for_each_entry(vma, &current->mm.vm->vm_list, vm_list), then you can use `vma` in your loop
        struct vm_area_struct * vma;
        ret.a0 = -1;
        list_for_each_entry(vma, &current->mm.vm->vm_list, vm_list){
            if(vma->vm_start == arg0 && vma->vm_end == arg0 + arg1){
                // 1. find the vma according to arg0 and arg1
                if(vma->mapped == 1){
                    // 2. if the vma mapped, free the physical pages (free_pages). Using `get_pte` to get the corresponding pte.
                    // 找到虚拟地址对应的物理地址，取消映射
                    uint64_t pte = get_pte(((current->satp)<<21)>>9,vma->vm_start);
                    free_pages((pte>>10)<<12);
                }
                // 3. ummap the physical pages from the virtual address space (create_mapping)
                create_mapping(((current->satp)<<21)>>9,vma->vm_start,0,(vma->vm_end-vma->vm_start),0);
                // 4. delete the vma from the mm_struct's vma list (list_del).
                list_del(&(vma->vm_list));
                // 5. free the vma struct (kfree).
                kfree(vma);
                // return 0 if success, otherwise return -1
                ret.a0 = 0;
                break;
            }
        }

        // flash the TLB
        asm volatile ("sfence.vma");
        break;

    }
    default:
        printf("Unknown syscall! syscall_num = %d\n", syscall_num);
        while(1);
        break;
    }
    return ret;
}