#ifndef TASK_H
#define TASK_H

#include "kernel.h"

typedef enum {
    TASK_STATE_READY,
    TASK_STATE_RUNNING,
    TASK_STATE_BLOCKED,
    TASK_STATE_SUSPENDED,
    TASK_STATE_TERMINATED
} task_state_t;

typedef void (*task_entry_t)(void *arg);

typedef struct task_control_block {
    uint32_t *stack_ptr;
    task_entry_t entry;
    void *arg;
    uint8_t priority;
    uint8_t base_priority; // For priority inheritance
    task_state_t state;
    uint32_t delay_ticks;  // For task_delay
    struct task_control_block *next; // Linked list pointer for queues
} tcb_t;

kernel_status_t task_create(tcb_t *tcb, task_entry_t entry, void *arg, uint8_t priority, uint32_t *stack, uint32_t stack_size);
kernel_status_t task_delay(uint32_t ticks);
kernel_status_t task_yield(void);
task_state_t task_get_state(tcb_t *tcb);

#endif // TASK_H
