#pragma once

#include "defs.h"
#include "slub.h"

#define PAGE_SIZE 4096UL

// 定义buddy system可分配的内存大小为16MB
#define MEMORY_SIZE 0x1000000

extern uint64_t _end;

static uint64_t alloc_page_num = 0;

typedef struct {
  bool initialized;
  uint64_t base_addr;
  unsigned int bitmap[8192];
} buddy;

static buddy buddy_system;

int alloced_page_num();

void init_buddy_system();

uint64_t alloc_pages(unsigned int num);

uint64_t alloc_page();

void free_pages(uint64_t pa);

void slub_init();

void memcpy(void * dst, void * src, size_t size);
