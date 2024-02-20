#include "defs.h"
extern struct sbiret sbi_call(uint64_t ext, uint64_t fid, uint64_t arg0,
                              uint64_t arg1, uint64_t arg2, uint64_t arg3,
                              uint64_t arg4, uint64_t arg5);

int puts(char *str) {
  // TODO
  char* s_ptr = str;
  while((*s_ptr)!=0){
    sbi_call(1,0,*s_ptr,0,0,0,0,0);
    s_ptr++;
  }
  return 0;
}

int put_num(uint64_t n) {
  // TODO
  char s[21] = {0};
  int i=0;
  if(n==0){
    s[i++] = '0';
  }else{
    while(n>0){
        s[i++] = '0'+n%10;
        n /= 10;
    }
  }

  int j;
  for(j=0;j<i/2;j++){
    char temp = s[j];
    s[j] = s[i-1-j];
    s[i-1-j] = temp;
  }
  puts(s);

  return 0;
}