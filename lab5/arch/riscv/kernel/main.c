#include "sched.h"
#include "stdio.h"
#include "test.h"
#include "sched.h"

int start_kernel() {
  // TODO: 各个线程怎么进入不同的main的？
  puts("ZJU OSLAB 5 3210105647 李力扬\n");
  
  slub_init();
  task_init();

  // 设置第一次时钟中断
  asm volatile("ecall");
  
  init_test_case();
  call_first_process();
  
  dead_loop();
  return 0;
}
