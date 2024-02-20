#pragma once

#include "syscall.h"

int fork();
void wait(int pid);
void exit(int ret);
void exec(const char * path);