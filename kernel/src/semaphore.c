#include "semaphore.h"
#include "scheduler.h"

kernel_status_t semaphore_init(semaphore_t *sem, uint32_t initial_count) {
    if (!sem) return KERNEL_INVALID_ARGUMENT;
    sem->count = initial_count;
    sem->wait_queue = NULL;
    return KERNEL_OK;
}

kernel_status_t semaphore_wait(semaphore_t *sem) {
    if (!sem) return KERNEL_INVALID_ARGUMENT;

    critical_section_enter();
    if (sem->count > 0) {
        sem->count--;
    } else {
        tcb_t *current = scheduler_get_current_task();
        scheduler_block_task(current);
        
        // Add to wait queue
        current->next = sem->wait_queue;
        sem->wait_queue = current;

        scheduler_request_context_switch();
    }
    critical_section_exit();

    return KERNEL_OK;
}

kernel_status_t semaphore_post(semaphore_t *sem) {
    if (!sem) return KERNEL_INVALID_ARGUMENT;

    critical_section_enter();
    if (sem->wait_queue != NULL) {
        // Pop task and unblock
        tcb_t *waiting = sem->wait_queue;
        sem->wait_queue = waiting->next;
        scheduler_unblock_task(waiting);
        scheduler_request_context_switch();
    } else {
        sem->count++;
    }
    critical_section_exit();

    return KERNEL_OK;
}
