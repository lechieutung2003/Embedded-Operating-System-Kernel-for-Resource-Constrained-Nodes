#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include "kernel.h"

typedef struct memory_block {
    struct memory_block *next;
} memory_block_t;

typedef struct {
    void *pool_start;
    size_t block_size;
    size_t num_blocks;
    memory_block_t *free_list;
} memory_pool_t;

kernel_status_t memory_pool_init(memory_pool_t *pool, void *buffer, size_t block_size, size_t num_blocks);
void* memory_pool_alloc(memory_pool_t *pool);
kernel_status_t memory_pool_free(memory_pool_t *pool, void *ptr);

#endif // MEMORY_POOL_H
