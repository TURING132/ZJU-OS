#include "sched.h"
#include "defs.h"
#include "mm.h"
#include "slub.h"
#include "stdio.h"
#include "task_manager.h"
#include "vm.h"

// If next==current,do nothing; else update current and call __switch_to.
void switch_to(struct task_struct *next) {
  if (current != next) {
    struct task_struct *prev = current;
    current = next;
    __switch_to(prev, next);
  }
}

void call_first_process() {
  current = (struct task_struct*)alloc_page();
  current->pid = -1;
  current->counter = 0;
  current->priority = 0;
  schedule();
}

void do_timer(void) {
  
}

// Select the next task to run. If all tasks are done(counter=0), set task0's
// counter to 1 and it would assign new test case.
void schedule(void) {
  unsigned char next;

  // DONE
  int min = 0x7fffffff;
  int min_p = 0x7fffffff;
  for (int i = NR_TASKS - 1; i >= 0; i--) {
    if (task[i] == 0x0) {
      continue;
    }
    if (task[i]->priority < min_p && task[i]->counter > 0) {
      min_p = task[i]->priority;
      min = task[i]->counter;
      next = i;
    } else if (task[i]->priority == min_p && task[i]->counter < min &&
               task[i]->counter > 0) {
      min = task[i]->counter;
      next = i;
    }
  }

  switch_to(task[next]);
}

void dead_loop() {
  while (1) {
  }
}
