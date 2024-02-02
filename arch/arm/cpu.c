#include "kernel.h"

static uint32_t primask_save;
static uint32_t critical_nesting = 0;

void critical_section_enter(void) {
    uint32_t primask;
    __asm volatile ("mrs %0, primask" : "=r" (primask));
    __asm volatile ("cpsid i");

    if (critical_nesting == 0) {
        primask_save = primask;
    }
    critical_nesting++;
}

void critical_section_exit(void) {
    if (critical_nesting == 0) return; // Error case

    critical_nesting--;
    if (critical_nesting == 0) {
        __asm volatile ("msr primask, %0" :: "r" (primask_save));
    }
}
