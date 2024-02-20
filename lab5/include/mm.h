#pragma once

#include "defs.h"

#define PAGE_SIZE 4096UL

// 定义buddy system可分配的内存大小为16MB
#define MEMORY_SIZE 0x1000000



extern uint64_t _end;


static uint64_t alloc_page_num = 0;

typedef struct {
  bool initialized;           // 是否完成初始化
  uint64_t base_addr;         // 管理的全部地址的起始地址
  unsigned int bitmap[8192];  // 满二叉树
} buddy;

static buddy buddy_system; // 整个buddy system只需要用一个结构体


int alloced_page_num(); // 已停用

void init_buddy_system();

uint64_t alloc_pages(unsigned int num);

uint64_t alloc_page();

void free_pages(uint64_t pa);


