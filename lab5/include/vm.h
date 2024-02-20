#pragma once

#include "defs.h"

# define PTE_V 0x001 // Valid
# define PTE_R 0x002 // Read
# define PTE_W 0x004 // Write
# define PTE_X 0x008 // Execute
# define PTE_U 0x010 // User


#define PHYSICAL_ADDR(x) (((uint64_t)(x)) & 0xffffffff | 0x80000000)
#define VIRTUAL_ADDR(x) (((uint64_t)(x)) & 0xfffffff | 0xffffffc000000000)


void create_mapping(uint64_t *pgtbl, uint64_t va, uint64_t pa, uint64_t sz,
                    int perm);

uint64_t get_pte(uint64_t *pgtbl, uint64_t va);

void paging_init();
