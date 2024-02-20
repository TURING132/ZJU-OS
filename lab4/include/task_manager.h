#pragma once
#include "defs.h"

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
extern struct task_struct* current;

/* 进程指针数组 */
extern struct task_struct* task[NR_TASKS];

extern int task_init_done;

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

/* 进程数据结构 */
struct task_struct {
  long state;                  // 进程状态 Lab3中进程初始化时置为TASK_RUNNING
  long counter;                // 运行剩余时间
  long priority;               // 运行优先级 1最高 5最低
  long blocked;
  long pid;                    // 进程标识符
                               // Above Size Cost: 40 bytes

  struct thread_struct thread; // 该进程状态段

  uint64_t sscratch; // 保存 sscratch
  uint64_t satp;     // 保存 satp

};

int getpid();

/* 进程初始化 创建四个dead_loop进程 */
void task_init(void);

extern void __init_sepc(void);

#endif