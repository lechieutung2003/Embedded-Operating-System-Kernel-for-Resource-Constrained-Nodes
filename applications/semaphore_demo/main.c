#include "kernel.h"
#include "task.h"
#include "semaphore.h"

semaphore_t data_sem;
volatile uint32_t data_value = 0;

static tcb_t producer_tcb;
static uint32_t producer_stack[256];

static tcb_t consumer_tcb;
static uint32_t consumer_stack[256];

void producer_entry(void *arg) {
    (void)arg;
    while(1) {
        data_value++;
        semaphore_post(&data_sem);
        task_delay(15);
    }
}

void consumer_entry(void *arg) {
    (void)arg;
    while(1) {
        semaphore_wait(&data_sem);
        // data_value is ready
        uint32_t read_val = data_value;
        (void)read_val; // Use it
    }
}

int main(void) {
    kernel_init();
    semaphore_init(&data_sem, 0);

    task_create(&producer_tcb, producer_entry, NULL, 2, producer_stack, sizeof(producer_stack));
    task_create(&consumer_tcb, consumer_entry, NULL, 1, consumer_stack, sizeof(consumer_stack)); // Consumer higher prio

    kernel_start();
    return 0;
}
