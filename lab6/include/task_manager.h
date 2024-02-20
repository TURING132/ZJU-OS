#pragma once
#include "defs.h"
#include "list.h"

#define TASK_SIZE (4096)
#define THREAD_OFFSET (5 * 0x08)

#ifndef __ASSEMBLER__

/* task的最大数量 */
#define NR_TASKS 64

#define FIRST_TASK (task[0])
#define LAST_TASK (task[NR_TASKS - 1])

/* 定义task的状态，lab3中task只需要一种状态。*/
#define TASK_RUNNING 0
// #define TASK_INTERRUPTIBLE       1
// #define TASK_UNINTERRUPTIBLE     2
// #define TASK_ZOMBIE              3
// #define TASK_STOPPED             4

#define PREEMPT_ENABLE 0
#define PREEMPT_DISABLE 1

/* lab3中进程的数量以及每个进程初始的时间片 */
#define LAB_TEST_NUM 5
#define LAB_TEST_COUNTER 5

/* 当前进程 */
extern struct task_struct *current;

/* 进程指针数组 */
extern struct task_struct *task[NR_TASKS];

/* 进程状态段数据结构 */
struct thread_struct {
  uint64_t ra;
  uint64_t sp;
  uint64_t s0;
  uint64_t s1;
  uint64_t s2;
  uint64_t s3;
  uint64_t s4;
  uint64_t s5;
  uint64_t s6;
  uint64_t s7;
  uint64_t s8;
  uint64_t s9;
  uint64_t s10;
  uint64_t s11;
};

/* 虚拟内存区域 */
typedef struct {
  unsigned long pgprot;
} pgprot_t;

struct vm_area_struct {
  /* Our start address within vm_area. */
  unsigned long vm_start;
  /* The first byte after our end address within vm_area. */
  unsigned long vm_end;
  /* linked list of VM areas per task, sorted by address. */
  struct list_head vm_list;
  // vm_page_prot和vm_flags的具体含义本实验不做要求，可以直接把vm_flags用于保存page_table的权限位。
  /* Access permissions of this VMA. */
  pgprot_t vm_page_prot;
  /* Flags*/
  unsigned long vm_flags;
  /* mapped */
  bool mapped;
};

/* 内存管理 */
struct mm_struct {
  struct vm_area_struct *vm;   // 虚拟内存区域描述符
  uint64_t user_program_start; // 进程起始地址（物理）
  uint64_t user_stack;         // 用户栈地址(物理)
};

/* 进程数据结构 */
struct task_struct {
  long state;    // 进程状态 Lab3中进程初始化时置为TASK_RUNNING
  long counter;  // 运行剩余时间
  long priority; // 运行优先级 1最高 5最低
  long blocked;
  long pid; // 进程标识符
            // Above Size Cost: 40 bytes

  struct thread_struct thread; // 该进程状态段

  uint64_t sscratch; // 保存 sscratch
  uint64_t satp;     // 保存 satp

  struct mm_struct mm;
};

int getpid();

/* 进程初始化 创建四个dead_loop进程 */
void task_init(void);

/* 用于返回用户态 */
extern void __init_sepc(void);

#endif