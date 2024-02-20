#include "mm.h"

#include "vm.h"
#include "stdio.h"


#define set_split(x) ((unsigned int)(x) | 0x80000000)   // 最高位决定是否split过
#define set_unsplit(x) ((unsigned int)(x) & 0x7fffffff) 
#define get_size(x) set_unsplit(x)                      // 返回size
#define check_split(x) ((unsigned int)(x) & 0x80000000) // 查看最高位


uint64_t get_index(uint64_t pa) {
  // 获取bitmap中，以物理地址 pa 起始的最上层的节点的index
  uint64_t offset = (pa - buddy_system.base_addr) / PAGE_SIZE; // 获得偏移了多少个page，以page_size为单位
  int block_size = 1;
  while(offset % block_size == 0) {
    block_size <<= 1;
  }
  block_size = block_size >> 1; // 最后一个能整除的就是这个块的block size
  return ((MEMORY_SIZE / PAGE_SIZE) / block_size) + (offset / block_size); //这一层之前的大小加上这一层的偏移量，TODO: 里面的数学关系
}

uint64_t get_addr(int index) {
  // 获取节点index的起始物理地址
  int level = 0, index_copy = index;
  while(index_copy > 1) {
    index_copy >>= 1;
    level++;
  } // 计算在第几层 13 6 3 1 level = 4

  int block_size = MEMORY_SIZE >> level;  // 在第几层size就block_size除几次2
  int offset = (index - (1 << level)) * block_size; // offset是在这一层的偏移量 offset = 5 * block_size
  return buddy_system.base_addr + offset;
}

uint32_t get_block_size(int index) {  
  // 获取index的block大小
  int level = 0, index_copy = index;
  while(index_copy > 1) {
    index_copy >>= 1;
    level++;
  }// 每向下一层block size减半
  return MEMORY_SIZE >> level;    
}


uint64_t alloc_page() {
  return alloc_pages(1);
}

int alloced_page_num() {
  // 返回已经分配的物理页面的数量
  // 在buddy system中，我们不再使用该函数
  return -1;
}

void init_buddy_system() {
  // TODO: 初始化buddy system
  // 1. 将buddy system的每个节点初始化为可分配的内存大小，单位为PAGE_SIZE
  // 注意我们用buddy_system.bitmap实现一个满二叉树，其下标变化规律如下：如果当前节点的下标是 X，那么左儿子就是 `X * 2` ，右儿子就是 `X * 2 + 1` ，X 从1开始。
  // 那么，下标为 X 的节点可分配的内存为多少呢？
  // 2. 将buddy system的base_addr设置为&_end的物理地址
  // 3. 将buddy system的initialized设置为true
  // 左儿子是*2，右儿子是*2+1
  // 父节点是/2
  int i;
  buddy_system.bitmap[1]=MEMORY_SIZE/PAGE_SIZE;
  for(i=2; i<8192; i++){
    buddy_system.bitmap[i] = buddy_system.bitmap[i/2]/2;
  }
  buddy_system.base_addr = PHYSICAL_ADDR((uint64_t)&_end);
  buddy_system.initialized = 1;
};

uint64_t alloc_buddy(int index, unsigned int num) {
  // 如果完成分配，则要递归更新节点信息，将涉及到的节点的可分配连续内存更新，
  // 已拆分则是左右子节点的可分配的最大连续物理内存最大值，未拆分则是两者之和，并使用set_split更新节点的状态

  // num应该是需要分配的大小，index应该是树根的位置
  // TODO: 找到可分配的节点并完成分配，返回分配的物理页面的首地址（通过get_addr函数可以获取节点index的起始地址）
  // 1. 如果当前节点大于num，则查看当前节点的左右儿子节点
  // 提示：通过get_size函数可以获取节点index的可分配连续内存大小
  if(get_size(buddy_system.bitmap[index])>num){
    uint64_t ret_addr = alloc_buddy(index*2,num);
    if(ret_addr==0){
      ret_addr = alloc_buddy(index*2+1,num);
    }
    if(ret_addr!=0){
      //已经做了分配，需要减小可用的size
      //更新可以连续分配的大小，应该是两个儿子里较大的一个
      uint64_t left_size = get_size(buddy_system.bitmap[index*2]);
      uint64_t right_size = get_size(buddy_system.bitmap[index*2+1]);
      buddy_system.bitmap[index] = set_split(left_size>right_size?left_size:right_size);
      return ret_addr;
    }
  }else if(get_size(buddy_system.bitmap[index])==num){//刚刚好，进行分配
    // 2. 如果当前节点的可分配连续内存等于num，且当前节点没有被拆分，则分配当前节点
    // 提示：通过check_split函数可以获取节点index的状态
    if(!check_split(buddy_system.bitmap[index])){
      buddy_system.bitmap[index] = 0;
      return get_addr(index);
    } 
    else{
    // 3. 如果当前节点的可分配连续内存等于num且已经被拆分，则查看当前节点的左右儿子节点
      uint64_t ret_addr = alloc_buddy(index*2,num);
      if(ret_addr==0){
        ret_addr = alloc_buddy(index*2+1,num);
      }
      buddy_system.bitmap[index] = 0; 
      return ret_addr;
    }
  }else{//不够大
    // 4. 如果当前节点的可分配连续内存小于num，则分配失败返回上层节点
    return 0;
  }
}

uint64_t alloc_pages(unsigned int num) {
  // 分配num个页面，返回分配到的页面的首地址，如果没有足够的空闲页面，返回0
  if (!buddy_system.initialized) {
    init_buddy_system();
  }
  // TODO:
  // 1. 将num向上对齐到2的幂次
  // 2. 调用alloc_buddy函数完成分配
  unsigned int up_num = 1;
  while(num>up_num){
    up_num = up_num<<1;
  }
  return alloc_buddy(1,up_num);
}


void free_buddy(int index) {
  // 提示：使用check_split函数可以获取节点index的状态
  // 提示：使用set_unsplit函数可以将节点index的状态恢复为初始状态
  // 提示：使用get_block_size函数可以获取节点index的初始可分配内存大小
  // TODO: 释放节点index的页面
  // 1. 首先判断节点index的状态，如果已经被拆分，则不能直接释放。异常状态，报错并进入死循环。
  if(check_split(buddy_system.bitmap[index])){
    printf("error: try to free splited index\n");
    while(1);
    return;
  }
  else{
    // 2. 如果没有被拆分，则恢复节点index的状态为初始状态
    buddy_system.bitmap[index] = get_block_size(index); // size回归为最初的大小
    buddy_system.bitmap[index] = set_unsplit(buddy_system.bitmap[index]);
    // 3. 如果该节点与其兄弟节点都没有被使用，则合并这两个节点，并递归处理父节点
    int bt_index = index; // back trace
    while(bt_index!=1){
      if(buddy_system.bitmap[bt_index] == buddy_system.bitmap[bt_index^1]){
        buddy_system.bitmap[bt_index/2] = get_block_size(bt_index/2);
        buddy_system.bitmap[bt_index/2] = set_unsplit(bt_index/2);
        bt_index /= 2; //向上回溯
      }
      else{
        break;
      }
    }
  }
}

void free_pages(uint64_t pa) {// 给定一个地主块的起始地址进行释放
  // TODO: find the buddy system node according to pa, and free it.
  // 注意，如果该节点的状态为已经被拆分，则应该释放其左子节点
  // 提示：使用get_index函数可以获取pa对应的最上层节点的下标
  int index = get_index(pa);
  while(check_split(buddy_system.bitmap[index])){
    // 因为直接获得的是最上级，应该往下找到第一个未被拆分的节点进行释放
    index = index * 2;
  }
  free_buddy(index);
  return;
}