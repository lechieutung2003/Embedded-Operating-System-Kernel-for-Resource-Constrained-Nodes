#include "scheduler.h"

#define ICSR (*(volatile uint32_t *)0xE000ED04)
#define PENDSVSET (1 << 28)

static tcb_t *ready_queues[NUM_PRIORITIES];
static tcb_t *current_task = NULL;
static tcb_t *next_task = NULL;

static tcb_t *delayed_list = NULL;

// Idle task
static tcb_t idle_tcb;
static uint32_t idle_stack[64];

static void idle_task(void *arg) {
    (void)arg;
    while(1) {
        // Wait for interrupt (WFI)
        __asm volatile ("wfi");
    }
}

void scheduler_init(void) {
    for (int i = 0; i < NUM_PRIORITIES; i++) {
        ready_queues[i] = NULL;
    }
    current_task = NULL;
    next_task = NULL;
    delayed_list = NULL;
    
    // Create idle task with lowest priority
    task_create(&idle_tcb, idle_task, NULL, NUM_PRIORITIES - 1, idle_stack, sizeof(idle_stack));
}

void scheduler_add_ready(tcb_t *task) {
    if (!task) return;
    task->state = TASK_STATE_READY;
    uint8_t prio = task->priority;
    task->next = NULL;

    if (ready_queues[prio] == NULL) {
        ready_queues[prio] = task;
    } else {
        tcb_t *curr = ready_queues[prio];
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = task;
    }
}

void scheduler_remove_ready(tcb_t *task) {
    if (!task) return;
    uint8_t prio = task->priority;
    if (ready_queues[prio] == task) {
        ready_queues[prio] = task->next;
    } else {
        tcb_t *curr = ready_queues[prio];
        while (curr != NULL && curr->next != task) {
            curr = curr->next;
        }
        if (curr != NULL) {
            curr->next = task->next;
        }
    }
    task->next = NULL;
}

static void schedule(void) {
    for (int i = 0; i < NUM_PRIORITIES; i++) {
        if (ready_queues[i] != NULL) {
            next_task = ready_queues[i];
            break;
        }
    }
    
    if (next_task != current_task) {
        if (current_task && current_task->state == TASK_STATE_RUNNING) {
            current_task->state = TASK_STATE_READY;
        }
        next_task->state = TASK_STATE_RUNNING;
    }
}

void scheduler_request_context_switch(void) {
    schedule();
    if (next_task != current_task) {
        ICSR |= PENDSVSET;
    }
}

tcb_t* scheduler_get_current_task(void) {
    return current_task;
}

tcb_t* scheduler_get_next_task(void) {
    return next_task;
}

void scheduler_start(void) {
    schedule();
    current_task = next_task;
    
    // Set PSP to the new task's stack pointer and trigger exception return to thread mode
    __asm volatile (
        "ldr r0, =current_task\n"
        "ldr r1, [r0]\n"
        "ldr r2, [r1]\n" // r2 = current_task->stack_ptr
        
        "ldmia r2!, {r4-r11}\n" // restore software saved registers
        "msr psp, r2\n"         // update PSP
        
        "mov r0, #3\n" // Thread mode, use PSP
        "msr control, r0\n"
        "isb\n"
        
        // Pop hardware saved registers explicitly since we're not returning from an exception yet
        "pop {r0-r3, r12, lr}\n" // r0-r3, r12, lr
        "pop {pc}\n" // PC and discard xPSR
    );
}

void scheduler_tick(void) {
    // Process delayed tasks
    tcb_t *prev = NULL;
    tcb_t *curr = delayed_list;
    uint8_t switch_req = 0;
    
    while (curr != NULL) {
        if (curr->delay_ticks > 0) {
            curr->delay_ticks--;
        }
        
        if (curr->delay_ticks == 0) {
            // Unblock task
            tcb_t *to_ready = curr;
            if (prev == NULL) {
                delayed_list = curr->next;
            } else {
                prev->next = curr->next;
            }
            curr = curr->next; // Move to next before modifying to_ready
            
            scheduler_add_ready(to_ready);
            switch_req = 1;
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
    
    if (switch_req || (ready_queues[current_task->priority] != current_task && ready_queues[current_task->priority] != NULL)) {
        // Round robin for same priority or higher priority became ready
        scheduler_request_context_switch();
    }
}

// Block and Unblock for general use (mutex/sem)
void scheduler_block_task(tcb_t *task) {
    if (!task) return;
    if (task->state == TASK_STATE_READY || task->state == TASK_STATE_RUNNING) {
        scheduler_remove_ready(task);
    }
    task->state = TASK_STATE_BLOCKED;
    if (task->delay_ticks > 0) {
        task->next = delayed_list;
        delayed_list = task;
    }
}

void scheduler_unblock_task(tcb_t *task) {
    if (!task) return;
    if (task->state == TASK_STATE_BLOCKED) {
        scheduler_add_ready(task);
    }
}

