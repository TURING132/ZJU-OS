#include "sched.h"
#include "defs.h"
#include "stdio.h"
#include "test.h"
#include "task_manager.h"

#define TEST_NUM 1

int ticks = 0;

// 该变量更改任务的时间片与优先级，同学们可以进行修改，自己的算法运行是否符合预期
// 建议多检测些边界条件，例如优先级时间片均相同的情况。
int counter_priority[3][LAB_TEST_NUM][2] = {
  {
    {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}
  },
  {
    {3, 1}, {2, 1}, {4, 1}, {1, 1}, {1, 2}
  },
  {
    {1, 1}, {1, 1}, {1, 1}, {1, 1}, {1, 1}
  }
};

void init_test_case() {
  static int c_p_i = 0;
  for (int i = 0; i < LAB_TEST_NUM; i++) {
    task[i]->counter = counter_priority[c_p_i][i][0];
    task[i]->priority = counter_priority[c_p_i][i][1];
  }
  if(c_p_i == TEST_NUM){
    printf("test end\n");
    // show the ticks
    printf("ticks: %d\n", ticks);
    while(1);
  }
  c_p_i++;
}
