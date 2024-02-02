#include "kernel.h"
#include "task.h"
#include "memory_pool.h"

#define NUM_BLOCKS 8
#define BLOCK_SIZE 32

uint8_t memory_buffer[NUM_BLOCKS * BLOCK_SIZE];
memory_pool_t my_pool;

static tcb_t task1_tcb;
static uint32_t task1_stack[256];

void task1_entry(void *arg) {
    (void)arg;
    while(1) {
        void *ptr1 = memory_pool_alloc(&my_pool);
        void *ptr2 = memory_pool_alloc(&my_pool);
        
        if (ptr1) {
            // Use block
        }
        
        if (ptr2) {
            // Use block
        }
        
        task_delay(5);
        
        if (ptr1) memory_pool_free(&my_pool, ptr1);
        if (ptr2) memory_pool_free(&my_pool, ptr2);
        
        task_delay(5);
    }
}

int main(void) {
    kernel_init();
    memory_pool_init(&my_pool, memory_buffer, BLOCK_SIZE, NUM_BLOCKS);

    task_create(&task1_tcb, task1_entry, NULL, 1, task1_stack, sizeof(task1_stack));

    kernel_start();
    return 0;
}
