#include "task.h"
#include "scheduler.h"

// Cortex-M Initial Stack layout
#define XPSR_DEFAULT 0x01000000 // Thumb bit set

kernel_status_t task_create(tcb_t *tcb, task_entry_t entry, void *arg, uint8_t priority, uint32_t *stack, uint32_t stack_size) {
    if (!tcb || !entry || !stack || stack_size < 32 || priority >= NUM_PRIORITIES) {
        return KERNEL_INVALID_ARGUMENT;
    }

    // Initialize TCB
    tcb->entry = entry;
    tcb->arg = arg;
    tcb->priority = priority;
    tcb->base_priority = priority;
    tcb->state = TASK_STATE_READY;
    tcb->delay_ticks = 0;
    tcb->next = NULL;

    // Initialize Stack
    // Cortex-M hardware pushes 8 registers on exception entry: xPSR, PC, LR, R12, R3, R2, R1, R0
    // We also need to save R4-R11 manually. Total = 16 words.
    uint32_t *sp = (uint32_t *)((uintptr_t)stack + stack_size);

    sp -= 16; // 16 registers

    // Hardware saved registers
    sp[15] = XPSR_DEFAULT; // xPSR
    sp[14] = (uint32_t)entry; // PC
    sp[13] = 0; // LR (can be mapped to a task_exit function)
    sp[12] = 0; // R12
    sp[11] = 0; // R3
    sp[10] = 0; // R2
    sp[9]  = 0; // R1
    sp[8]  = (uint32_t)arg; // R0

    // Software saved registers (R4-R11)
    sp[7] = 0; // R11
    sp[6] = 0; // R10
    sp[5] = 0; // R9
    sp[4] = 0; // R8
    sp[3] = 0; // R7
    sp[2] = 0; // R6
    sp[1] = 0; // R5
    sp[0] = 0; // R4

    tcb->stack_ptr = sp;

    // Add to ready queue
    critical_section_enter();
    scheduler_add_ready(tcb);
    critical_section_exit();

    return KERNEL_OK;
}

kernel_status_t task_delay(uint32_t ticks) {
    critical_section_enter();
    tcb_t *current = scheduler_get_current_task();
    if (current) {
        current->delay_ticks = ticks;
        current->state = TASK_STATE_BLOCKED;
        scheduler_remove_ready(current);
        scheduler_request_context_switch();
    }
    critical_section_exit();
    return KERNEL_OK;
}

kernel_status_t task_yield(void) {
    scheduler_request_context_switch();
    return KERNEL_OK;
}

task_state_t task_get_state(tcb_t *tcb) {
    if (!tcb) return TASK_STATE_TERMINATED;
    return tcb->state;
}
