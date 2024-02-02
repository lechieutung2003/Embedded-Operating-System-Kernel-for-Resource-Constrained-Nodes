#include "memory_pool.h"

kernel_status_t memory_pool_init(memory_pool_t *pool, void *buffer, size_t block_size, size_t num_blocks) {
    if (!pool || !buffer || block_size < sizeof(memory_block_t) || num_blocks == 0) {
        return KERNEL_INVALID_ARGUMENT;
    }

    pool->pool_start = buffer;
    pool->block_size = block_size;
    pool->num_blocks = num_blocks;
    pool->free_list = (memory_block_t *)buffer;

    // Initialize free list
    memory_block_t *current = pool->free_list;
    for (size_t i = 0; i < num_blocks - 1; i++) {
        current->next = (memory_block_t *)((uintptr_t)current + block_size);
        current = current->next;
    }
    current->next = NULL; // Last block

    return KERNEL_OK;
}

void* memory_pool_alloc(memory_pool_t *pool) {
    if (!pool) return NULL;

    critical_section_enter();
    if (pool->free_list == NULL) {
        critical_section_exit();
        return NULL; // Out of memory
    }

    memory_block_t *block = pool->free_list;
    pool->free_list = block->next;
    critical_section_exit();

    return (void *)block;
}

kernel_status_t memory_pool_free(memory_pool_t *pool, void *ptr) {
    if (!pool || !ptr) return KERNEL_INVALID_ARGUMENT;

    // Defensive check: is ptr within bounds and aligned?
    uintptr_t p = (uintptr_t)ptr;
    uintptr_t start = (uintptr_t)pool->pool_start;
    uintptr_t end = start + (pool->block_size * pool->num_blocks);

    if (p < start || p >= end || ((p - start) % pool->block_size) != 0) {
        return KERNEL_INVALID_ARGUMENT;
    }

    critical_section_enter();
    
    // Simple double-free detection: traverse free list
    memory_block_t *curr = pool->free_list;
    while (curr != NULL) {
        if (curr == (memory_block_t *)ptr) {
            critical_section_exit();
            return KERNEL_ERROR; // Double free detected
        }
        curr = curr->next;
    }

    memory_block_t *block = (memory_block_t *)ptr;
    block->next = pool->free_list;
    pool->free_list = block;

    critical_section_exit();
    return KERNEL_OK;
}
