    .syntax unified
    .cpu cortex-m3
    .fpu softvfp
    .thumb

    .global PendSV_Handler
    .extern scheduler_get_current_task
    .extern scheduler_get_next_task

    .section .text.PendSV_Handler
    .type PendSV_Handler, %function
PendSV_Handler:
    /* Disable interrupts to protect context switch */
    cpsid i

    /* Get current task's stack pointer (PSP) */
    mrs r0, psp
    cbz r0, restore_context /* If PSP is 0, this is the very first context switch or idle */

    /* Save software-saved registers (r4-r11) onto current task's stack */
    stmdb r0!, {r4-r11}

    /* Save the updated PSP to the current task's TCB */
    ldr r1, =scheduler_get_current_task
    blx r1
    /* r0 now contains pointer to current_task TCB */
    mrs r1, psp
    subs r1, r1, #32 /* Because we saved 8 words (32 bytes) */
    str r1, [r0] /* current_task->stack_ptr is the first field in tcb_t */

restore_context:
    /* Get next task's TCB */
    ldr r1, =scheduler_get_next_task
    blx r1
    /* r0 now contains pointer to next_task TCB */

    /* Load the next task's stack pointer from its TCB */
    ldr r1, [r0]

    /* Restore software-saved registers (r4-r11) from the next task's stack */
    ldmia r1!, {r4-r11}

    /* Update PSP with the new stack pointer */
    msr psp, r1

    /* Enable interrupts */
    cpsie i

    /* Exception return, returning to Thread mode using PSP */
    ldr r0, =0xFFFFFFFD
    bx r0

    .size PendSV_Handler, .-PendSV_Handler
