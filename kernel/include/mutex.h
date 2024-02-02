#ifndef MUTEX_H
#define MUTEX_H

#include "kernel.h"
#include "task.h"

typedef struct {
    uint8_t locked;
    tcb_t *owner;
    tcb_t *wait_queue; // Linked list of tasks waiting
} mutex_t;

kernel_status_t mutex_init(mutex_t *mutex);
kernel_status_t mutex_lock(mutex_t *mutex);
kernel_status_t mutex_unlock(mutex_t *mutex);

#endif // MUTEX_H
