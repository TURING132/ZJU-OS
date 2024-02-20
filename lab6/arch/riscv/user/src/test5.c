#include "getchar.h"
#include "getpid.h"
#include "mm.h"
#include "stdio.h"
#include <stdio.h>

int getchar_until_valid();
int getnumber();

int main() {
  printf("[process test start]\n");
  int secret, guess, count;
  secret = 37;
  count = 0;
  printf("I thought of a number between 1 and 100, can you guess it?\n");
  do {
    printf("I guess：");
    guess = getnumber();
    if (guess == -1) {
      printf("Invalid input\n");
      continue;
    }
    count++;
    if (guess > secret) {
      printf("too big\n");
    } else if (guess < secret) {
      printf("too small\n");
    }
  } while (guess != secret);
  printf("Congratulations, you guessed it right! You have guessed %d times in total.\n", count);
  return 0;
}

int getchar_until_valid() {
  int ch;
  do {
    ch = getchar();
  } while (ch <= 0);
  return ch;
}

int getnumber() {
  char input[64];
  int n = 0, ch;
  for (;;) {
    ch = getchar_until_valid();
    printf("%c", ch);

    switch (ch) {
    case '\r' /* enter */:
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
  
  int num = 0;
  for (int i = 0; i < n; i++) {
    if (input[i] >= '0' && input[i] <= '9') {
      num = num * 10 + input[i] - '0';
    } else {
      return -1;
    }
  }

  return num;
}