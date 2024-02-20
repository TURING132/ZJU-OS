#pragma once

#include "stddef.h"

#define Log(format, ...)                                          \
  printf("[%s:%d %s] " format "\n", __FILE__, __LINE__, __func__, \
         ##__VA_ARGS__);

int printf(const char *, ...);
int putchar(int);
int puts(const char *);