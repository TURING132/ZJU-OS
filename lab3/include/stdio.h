#pragma once

#include "stddef.h"

#define Log(format, ...)                                          \
  printf("[%s:%d %s] " format "\n", __FILE__, __LINE__, __func__, \
         ##__VA_ARGS__);

#define UART16550A_DR (volatile unsigned char *)0x10000000
  
int printf(const char *, ...);
int putchar(const char);
int puts(const char *);
