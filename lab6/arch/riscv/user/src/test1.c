#include "proc.h"
#include "stdio.h"
#include "getpid.h"
#include "getchar.h"

int strcmp(const char *a, const char *b);
int getchar_until_valid();


int main() {
  char program[][10] = {"hello", "malloc", "print", "guess"};
  char input[64];
  int n = 0, ch;

  for (;;) {
    n = 0;

    printf("lab6@oslab $ ");
    for (;;) {
      ch = getchar_until_valid();
      printf("%c", ch);

      switch (ch) {
        case '\r' /* enter */ :
          // note that: enter_key --> \r\n
          printf("\n");
          goto input_end;
        case 127 /* delete */:
          if (n) {
            n--;
            printf("\b \b");
          }
          break;
        default:
          input[n++] = ch;
      }
    }
input_end:
    input[n] = '\0';

    // exec user's instruction
    if (strcmp(input, "ls") == 0) {
      for (int i = 0; i < 4; i++) {
        printf("%s ", program[i]);
      }
      printf("\n");
    } else {
      for (int i = 0; i < 4; i++) {
        if (strcmp(input, program[i]) == 0) {
          int ret = fork();
          if (ret == 0) {
            // child process
            exec(program[i]);
          } else {
            // main process
            wait(ret);
          }
        }
      }
    }
  }

  return 0;
}


int strcmp(const char *a, const char *b) {
  while (*a && *b) {
    if (*a < *b)
      return -1;
    if (*a > *b)
      return 1;
    a++;
    b++;
  }
  if (*a && !*b)
    return 1;
  if (*b && !*a)
    return -1;
  return 0;
}

int getchar_until_valid() {
  int ch;
  do {
    ch = getchar();
  } while (ch <= 0);
  return ch;
}

