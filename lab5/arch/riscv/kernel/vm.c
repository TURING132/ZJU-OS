#include "vm.h"
#include "mm.h"
#include "sched.h"
#include "stdio.h"
#include "test.h"

extern uint64_t text_start;
extern uint64_t rodata_start;
extern uint64_t data_start;
extern uint64_t _end;
extern uint64_t user_program_start;



void create_mapping(uint64_t *pgtbl, uint64_t va, uint64_t pa, uint64_t sz,
                    int perm) {
  // pgtbl 为根页表的基地址
  // va，pa 分别为需要映射的虚拟、物理地址的基地址
  // sz 为映射的大小，单位为字节
  // perm 为映射的读写权限

  // pgtbl 是根页表的首地址，它已经占用了一页的大小，不需要再分配
  // （调用create_mapping前需要先分配好根页表）

  // 1. 通过 va 得到一级页表项的索引
  // 2. 通过 va 得到二级页表项的索引
  // 3. 通过 va 得到三级页表项的索引
  // 4. 如果一级页表项不存在，分配一个二级页表
  // 5. 设置一级页表项的内容
  // 6. 如果二级页表项不存在，分配一个三级页表
  // 7. 设置二级页表项的内容
  // 8. 设置三级页表项的内容

  // DONE: 请完成你的代码
  int page_num = (sz - 1) / PAGE_SIZE + 1;

  for(int i = 0; i < page_num; ++i) {

    uint64_t va_i = va + i * PAGE_SIZE;
    uint64_t pa_i = pa + i * PAGE_SIZE;

    // 通过 va 得到一级页表项的索引
    uint64_t first_index = (va_i >> 30) & 0x1ff;
    // 通过 va 得到二级页表项的索引
    uint64_t second_index = (va_i >> 21) & 0x1ff;
    // 通过 va 得到三级页表项的索引
    uint64_t third_index = (va_i >> 12) & 0x1ff;

    // 如果一级页表项不存在，分配一个物理页面
    if ((pgtbl[first_index] & PTE_V) == 0) {
      // 分配一个物理页面
      uint64_t *second_page = (uint64_t *)alloc_page();
      // 设置一级页表项的内容
      pgtbl[first_index] = (((uint64_t)second_page >> 12) << 10) | PTE_V /*| (perm & (PTE_U | PTE_V))*/;
    }
    // 通过一级页表项的内容得到二级页表的首地址
    uint64_t *second_page = (uint64_t *)((pgtbl[first_index] >> 10) << 12);
    // 如果二级页表项不存在，分配一个物理页面
    if ((second_page[second_index] & PTE_V) == 0) {
      // 分配一个物理页面
      uint64_t *third_page = (uint64_t *)alloc_page();
      // 设置二级页表项的内容
      second_page[second_index] = (((uint64_t)third_page >> 12) << 10) | (perm & (PTE_U | PTE_V));
    }
    // 通过二级页表项的内容得到三级页表的首地址
    uint64_t *third_page = (uint64_t *)((second_page[second_index] >> 10) << 12);
    // 设置三级页表项的内容
    third_page[third_index] = ((pa_i >> 12) << 10) | perm;
  }
}

uint64_t get_pte(uint64_t *pgtbl, uint64_t va) {

  // 通过 va 得到一级页表项的索引
  uint64_t first_index = (va >> 30) & 0x1ff;
  // 通过 va 得到二级页表项的索引
  uint64_t second_index = (va >> 21) & 0x1ff;
  // 通过 va 得到三级页表项的索引
  uint64_t third_index = (va >> 12) & 0x1ff;

  if ((pgtbl[first_index] & PTE_V) == 0) {    
    return 0;
  }
  // 通过一级页表项的内容得到二级页表的首地址
  uint64_t *second_page = (uint64_t *)((pgtbl[first_index] >> 10) << 12);
  
  if ((second_page[second_index] & PTE_V) == 0) {
    return 0;
  }
  // 通过二级页表项的内容得到三级页表的首地址
  uint64_t *third_page = (uint64_t *)((second_page[second_index] >> 10) << 12);
  
  return third_page[third_index];
}

void paging_init() { 
  // 在 vm.c 中编写 paging_init 函数，该函数完成以下工作：
  // 1. 创建内核的虚拟地址空间，调用 create_mapping 函数将虚拟地址 0xffffffc000000000 开始的 16 MB 空间映射到起始物理地址为 0x80000000 的 16MB 空间，PTE_V | PTE_R | PTE_W | PTE_X 为映射的读写权限。
  // 2. 对内核起始地址 0x80000000 的16MB空间做等值映射（将虚拟地址 0x80000000 开始的 16 MB 空间映射到起始物理地址为 0x80000000 的 16MB 空间），PTE_V | PTE_R | PTE_W | PTE_X 为映射的读写权限。
  // 3. 修改对内核空间不同 section 所在页属性的设置，完成对不同section的保护，其中text段的权限为 r-x, rodata 段为 r--, 其他段为 rw-，注意上述两个映射都需要做保护。
  // 4. 将必要的硬件地址（如 0x10000000 为起始地址的 UART ）进行等值映射 ( 可以映射连续 1MB 大小 )，无偏移，PTE_V | PTE_R 为映射的读写权限

  // 注意：paging_init函数创建的页表只用于内核开启页表之后，进入第一个用户进程之前。进入第一个用户进程之后，就会使用进程页表，而不再使用 paging_init 创建的页表。

  uint64_t *pgtbl = alloc_page();
  // DONE: 请完成你的代码
  // 调用 create_mapping 函数将虚拟地址 0xffffffc000000000 开始的 16 MB 空间映射到起始物理地址为 0x80000000 的 16MB 空间
  create_mapping(pgtbl, 0xffffffc000000000, 0x80000000, 16 * 1024 * 1024, PTE_V | PTE_R | PTE_W | PTE_X);
  // 修改对内核空间不同 section 所在页属性的设置，完成对不同section的保护，其中text段的权限为 r-x, rodata 段为 r--, 其他段为 rw-。
  create_mapping(pgtbl, 0xffffffc000000000, 0x80000000, (uint64_t)&rodata_start - 0x80000000, PTE_V | PTE_R | PTE_X);
  create_mapping(pgtbl, (uint64_t)&rodata_start-0x80000000+0xffffffc000000000, (uint64_t)&rodata_start, (uint64_t)&data_start - (uint64_t)&rodata_start, PTE_V | PTE_R);
  create_mapping(pgtbl, (uint64_t)&data_start-0x80000000+0xffffffc000000000, (uint64_t)&data_start, (uint64_t)&_end - (uint64_t)&data_start, PTE_V | PTE_R | PTE_W);
  // 对内核起始地址 0x80000000 的16MB空间做等值映射（将虚拟地址 0x80000000 开始的 16 MB 空间映射到起始物理地址为 0x80000000 的 16MB 空间）
  create_mapping(pgtbl, 0x80000000, 0x80000000, 16 * 1024 * 1024, PTE_V | PTE_R | PTE_W | PTE_X);
  // 修改对内核空间不同 section 所在页属性的设置，完成对不同section的保护，其中text段的权限为 r-x, rodata 段为 r--, 其他段为 rw-。
  create_mapping(pgtbl, 0x80000000, 0x80000000, (uint64_t)&rodata_start - 0x80000000, PTE_V | PTE_R | PTE_X);
  create_mapping(pgtbl, (uint64_t)&rodata_start, (uint64_t)&rodata_start, (uint64_t)&data_start - (uint64_t)&rodata_start, PTE_V | PTE_R);
  create_mapping(pgtbl, (uint64_t)&data_start, (uint64_t)&data_start, (uint64_t)&_end - (uint64_t)&data_start, PTE_V | PTE_R | PTE_W);
  // 将必要的硬件地址（如 0x10000000 为起始地址的 UART ）进行等值映射 ( 可以映射连续 1MB 大小 )，无偏移，3 为映射的读写权限
  create_mapping(pgtbl, 0x10000000, 0x10000000, 1 * 1024 * 1024, PTE_V | PTE_R | PTE_W | PTE_X);
  
}


