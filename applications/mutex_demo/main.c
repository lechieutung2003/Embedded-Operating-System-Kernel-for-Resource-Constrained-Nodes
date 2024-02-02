#include "kernel.h"
#include "task.h"
#include "mutex.h"

volatile uint32_t shared_resource = 0;
mutex_t my_mutex;

static tcb_t task1_tcb;
static uint32_t task1_stack[256];

static tcb_t task2_tcb;
static uint32_t task2_stack[256];

void task1_entry(void *arg) {
    (void)arg;
    while(1) {
        mutex_lock(&my_mutex);
        shared_resource++;
        task_delay(5); // Hold mutex for a while
        mutex_unlock(&my_mutex);
        task_delay(10);
    }
}

void task2_entry(void *arg) {
    (void)arg;
    while(1) {
        mutex_lock(&my_mutex);
        shared_resource++;
        mutex_unlock(&my_mutex);
        task_delay(5);
    }
}

int main(void) {
    kernel_init();
    mutex_init(&my_mutex);

    // Create Task 1
    task_create(&task1_tcb, task1_entry, NULL, 2, task1_stack, sizeof(task1_stack));

    // Create Task 2
    task_create(&task2_tcb, task2_entry, NULL, 2, task2_stack, sizeof(task2_stack));

    kernel_start();
    return 0;
}
