#pragma once

#include "defs.h"

#define offset (0xffffffc000000000 - 0x80000000)

# define PTE_V 0x001 // Valid
# define PTE_R 0x002 // Read
# define PTE_W 0x004 // Write
# define PTE_X 0x008 // Execute
# define PTE_U 0x010 // User

#define PAGE_SIZE 4096UL

#define PHYSICAL_ADDR(x) (((uint64_t)(x)) & 0xffffffff | 0x80000000)
#define VIRTUAL_ADDR(x) (((uint64_t)(x)) & 0xfffffff | 0xffffffc000000000)

static uint64_t alloc_page_num = 0;

uint64_t alloc_page();

int alloced_page_num();

void create_mapping(uint64_t *pgtbl, uint64_t va, uint64_t pa, uint64_t sz,
                    int perm);

void paging_init();
