#include "kernel.h"
#include "task.h"
#include "memory_pool.h"
#include "uart.h"

#define POOL_BLOCKS 4
#define POOL_BLOCK_SIZE 32
static uint8_t pool_mem[POOL_BLOCKS * POOL_BLOCK_SIZE];
static memory_pool_t mem_pool;

volatile uint32_t task1_counter = 0;
volatile uint32_t task2_counter = 0;

static tcb_t task1_tcb;
static uint32_t task1_stack[256];

static tcb_t task2_tcb;
static uint32_t task2_stack[256];

void task1_entry(void *arg) {
    (void)arg;
    while(1) {
        task1_counter++;
        uart_puts("[TASK 1] Priority: 1 (High) | Iteration: ");
        uart_print_dec(task1_counter);
        uart_puts(" | Allocating pool block...\n");

        void *p = memory_pool_alloc(&mem_pool);
        if (p) {
            uart_puts("         [MEM_POOL] Block allocated at ");
            uart_print_hex((uint32_t)p);
            uart_puts(" (Deterministic O(1), 0% fragmentation)\n");

            // Release back to pool
            memory_pool_free(&mem_pool, p);
            uart_puts("         [MEM_POOL] Block freed back to pool\n");
        }

        uart_puts("[TASK 1] Delaying 10 ticks -> Context switch to Task 2\n\n");
        task_delay(10);
    }
}

void task2_entry(void *arg) {
    (void)arg;
    while(1) {
        task2_counter++;
        uart_puts("[TASK 2] Priority: 2 (Normal) | Iteration: ");
        uart_print_dec(task2_counter);
        uart_puts(" | Executing background telemetry processing\n");
        uart_puts("[TASK 2] Delaying 20 ticks -> Yielding CPU\n\n");
        task_delay(20);
    }
}

int main(void) {
    uart_init();
    uart_puts("\n============================================================\n");
    uart_puts("  Embedded RTOS Kernel - Resource-Constrained ARM Cortex-M3\n");
    uart_puts("  Target: QEMU lm3s6965evb | Architecture: ARMv7-M\n");
    uart_puts("============================================================\n");
    uart_puts("[KERNEL] Initializing kernel subsystems...\n");
    kernel_init();

    memory_pool_init(&mem_pool, pool_mem, POOL_BLOCK_SIZE, POOL_BLOCKS);
    uart_puts("[KERNEL] Fixed-size memory pool initialized (32B x 4 blocks)\n");

    uart_puts("[KERNEL] Spawning Task 1 (Priority 1) & Task 2 (Priority 2)...\n");
    task_create(&task1_tcb, task1_entry, NULL, 1, task1_stack, sizeof(task1_stack));
    task_create(&task2_tcb, task2_entry, NULL, 2, task2_stack, sizeof(task2_stack));

    uart_puts("[KERNEL] Starting Preemptive Priority-Based Scheduler...\n\n");
    kernel_start();

    return 0;
}
