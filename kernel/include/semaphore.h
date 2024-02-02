#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "kernel.h"
#include "task.h"

typedef struct {
    uint32_t count;
    tcb_t *wait_queue; // Linked list of tasks waiting
} semaphore_t;

kernel_status_t semaphore_init(semaphore_t *sem, uint32_t initial_count);
kernel_status_t semaphore_wait(semaphore_t *sem);
kernel_status_t semaphore_post(semaphore_t *sem);

#endif // SEMAPHORE_H
