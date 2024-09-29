#include "stdlib.h"
#include "reset_allocator.h"
/*
 * Tries to allocate 'size' bytes for the allocator.
 * MEM_Reset_Allocator.base == 0 if allocation was unsuccessful.
*/
MEM_Reset_Allocator MEM_reset_allocator_init(size_t size){
  struct MEM_Reset_Allocator allocator;
  allocator.base = calloc(1, size);
  allocator.top = allocator.base;
  allocator.size = size;
  allocator.failed_allocs = 0;
  return allocator;
}

/*
 * Tries to allocate 'size' bytes of memory.
 * Returns NULL if unsuccessful.
*/
void* MEM_reset_allocator_alloc(MEM_Reset_Allocator* allocator, size_t size){
  if(allocator == NULL){
    return NULL;
  }else if(allocator->base == NULL){
    return NULL;
  }else if ((size_t)((char*)allocator->base + allocator->size - (char*)allocator->top) < size){
    allocator->failed_allocs++;
    return NULL;
  }
  void* ret = allocator->top;
  allocator->top = (char*)allocator->top + size;
  return ret;
}

/*
 * Resets the 'top' pointer to equal base.
 * No pointers allocated before reset should not be used.
*/
void MEM_reset_allocator_reset(MEM_Reset_Allocator* allocator){
  if(allocator == NULL){
    return;
  }
  allocator->top = allocator->base;
  allocator->failed_allocs = 0;
}

/*
 * Frees all memory and sets all pointers to NULL.
*/
void MEM_reset_allocator_destroy(MEM_Reset_Allocator* allocator){
  if(allocator == NULL){
    return;
  }else if (allocator-> base == NULL){
    return;
  }
  free(allocator->base);
  allocator->base = NULL;
  allocator->top = NULL;
  allocator->size = 0;
}
