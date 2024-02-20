typedef unsigned long long uint64_t;
extern puts();
#ifndef PRINT_ONLY
extern init();
extern get_cycles();
#endif
extern struct sbiret sbi_call(uint64_t ext, uint64_t fid, uint64_t arg0,
                              uint64_t arg1, uint64_t arg2, uint64_t arg3,
                              uint64_t arg4, uint64_t arg5);

int main() {
  int a = 0;
#ifndef PRINT_ONLY
  init();
#else
  put_num(2023);
  puts(" Hello Oslab!\n");
#endif
  while (1) {
    if (a++ > 10000) {
      a = 0;
    }
  }
  return 0;
}