#pragma once
#include "task_manager.h"

#ifndef __ASSEMBLER__


/* 在时钟中断处理中被调用 */
void do_timer(void);

/* 调度程序 */
void schedule(void);

/* 切换当前任务current到下一个任务next */
void switch_to(struct task_struct* next);

extern void __switch_to(struct task_struct* prev, struct task_struct* next);

/* 死循环 */
void dead_loop(void);

#endif
