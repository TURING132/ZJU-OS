#include "sched.h"
#include "stdio.h"
#include "test.h"
#include "defs.h"
#include "vm.h"
#include "task_manager.h"




// If next==current,do nothing; else update current and call __switch_to.
void switch_to(struct task_struct* next) {
  if (current != next) {
    struct task_struct* prev = current;
    current = next;
    __switch_to(prev, next);
  }
}

void call_first_process() {
  // set current to 0x0 and call schedule()
  current = (struct task_struct*)alloc_page();
  current->pid = -1;
  current->counter = 0;
  current->priority = 0;

  schedule();
}


void show_schedule(unsigned char next) {
  // show the information of all task and mark out the next task to run
  for (int i = 0; i < LAB_TEST_NUM; ++i) {
    if (task[i]->pid == next) {
      printf("task[%d]: counter = %d, priority = %d <-- next\n", i,
             task[i]->counter, task[i]->priority);
    } else {
      printf("task[%d]: counter = %d, priority = %d\n", i, task[i]->counter,
           task[i]->priority);
    }
  }
}


// simulate the cpu timeslice, which measn a short time frame that gets assigned
// to process for CPU execution
void do_timer(void) {
  if (!task_init_done) return;

  printf("[*PID = %d] Context Calculation: counter = %d,priority = %d\n",
         current->pid, current->counter, current->priority);
  
  // current process's counter -1, judge whether to schedule or go on.
  // DONE: if current process's counter is 0, schedule.
  current->counter--;
  if (current->counter == 0) {
    schedule();
  }

}

// Select the next task to run. If all tasks are done(counter=0), set task0's
// counter to 1 and it would assign new test case.
void schedule(void) {
  unsigned char next;
  // DONE: select the next task to run
  int min = 0x7fffffff;
  for(int pid = NR_TASKS-1; pid >= 0; pid--) {
    if(task[pid] == 0x0) {
      continue;
    }
    if(task[pid]->counter < min & task[pid]->counter > 0) {
      min = task[pid]->counter;
      next = task[pid]->pid;
    }
  }
  if (min == 0x7fffffff) {
    init_test_case();
    for(int pid = NR_TASKS-1; pid >= 0; pid--) {
      if(task[pid] == 0x0) {
        continue;
      }
      if(task[pid]->counter < min & task[pid]->counter > 0) {
        min = task[pid]->counter;
        next = task[pid]->pid;
      }
    }
  }

  show_schedule(next);
  switch_to(task[next]);
  
}

void dead_loop() {
  while (1) {
  }
}
