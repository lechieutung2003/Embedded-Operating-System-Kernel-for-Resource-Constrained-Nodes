#include "kernel.h"
#include "task.h"

// Define a simple volatile counter to observe execution
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
        task_delay(10); // Block for 10 ticks
    }
}

void task2_entry(void *arg) {
    (void)arg;
    while(1) {
        task2_counter++;
        task_delay(20); // Block for 20 ticks
    }
}

int main(void) {
    kernel_init();

    // Create Task 1: Higher priority (lower number)
    task_create(&task1_tcb, task1_entry, NULL, 1, task1_stack, sizeof(task1_stack));

    // Create Task 2: Lower priority
    task_create(&task2_tcb, task2_entry, NULL, 2, task2_stack, sizeof(task2_stack));

    // Start scheduler (does not return)
    kernel_start();

    return 0;
}
