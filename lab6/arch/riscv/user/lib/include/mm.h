#pragma once

#include "types.h"

#define PTE_V 0x001 // Valid
#define PTE_R 0x002 // Read
#define PTE_W 0x004 // Write
#define PTE_X 0x008 // Execute
#define PTE_U 0x010 // User

void *mmap(void *__addr, size_t __len, int __prot, int __flags, int __fd,
           __off_t __offset);

int munmap(void *__addr, size_t __len);
