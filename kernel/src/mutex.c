#include "mutex.h"
#include "scheduler.h"

kernel_status_t mutex_init(mutex_t *mutex) {
    if (!mutex) return KERNEL_INVALID_ARGUMENT;
    mutex->locked = 0;
    mutex->owner = NULL;
    mutex->wait_queue = NULL;
    return KERNEL_OK;
}

kernel_status_t mutex_lock(mutex_t *mutex) {
    if (!mutex) return KERNEL_INVALID_ARGUMENT;

    critical_section_enter();
    tcb_t *current = scheduler_get_current_task();

    if (mutex->locked == 0) {
        // Mutex is free
        mutex->locked = 1;
        mutex->owner = current;
    } else {
        // Mutex is locked. Implement Priority Inheritance
        if (mutex->owner->priority > current->priority) {
            // Priority is lower number = higher priority
            // Owner has lower priority than current task, boost owner priority
            scheduler_remove_ready(mutex->owner);
            mutex->owner->priority = current->priority;
            scheduler_add_ready(mutex->owner);
        }

        // Block current task and add to wait queue
        scheduler_block_task(current);

        // Add to mutex wait queue (simple LIFO or FIFO, we use LIFO for simplicity here)
        current->next = mutex->wait_queue;
        mutex->wait_queue = current;

        scheduler_request_context_switch();
    }
    critical_section_exit();

    return KERNEL_OK;
}

kernel_status_t mutex_unlock(mutex_t *mutex) {
    if (!mutex) return KERNEL_INVALID_ARGUMENT;

    critical_section_enter();
    tcb_t *current = scheduler_get_current_task();

    if (mutex->locked == 1 && mutex->owner == current) {
        // Restore original priority if inherited
        if (current->priority != current->base_priority) {
            scheduler_remove_ready(current);
            current->priority = current->base_priority;
            scheduler_add_ready(current);
        }

        if (mutex->wait_queue != NULL) {
            // Pop from wait queue
            tcb_t *waiting = mutex->wait_queue;
            mutex->wait_queue = waiting->next;

            // Transfer ownership
            mutex->owner = waiting;
            scheduler_unblock_task(waiting);
            
            // If waiting task has higher priority, we need to context switch
            scheduler_request_context_switch();
        } else {
            mutex->locked = 0;
            mutex->owner = NULL;
        }
    }
    critical_section_exit();

    return KERNEL_OK;
}
