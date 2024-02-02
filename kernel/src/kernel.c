#include "kernel.h"
#include "scheduler.h"

// Define a simple CPU frequency and SysTick configuration (e.g. 1ms tick)
#define CPU_FREQ 16000000 // default for QEMU lm3s6965evb if not reconfigured
#define SYSTICK_RELOAD (CPU_FREQ / 1000 - 1)

// SysTick registers
#define SYSTICK_CSR  (*(volatile uint32_t *)0xE000E010)
#define SYSTICK_RVR  (*(volatile uint32_t *)0xE000E014)
#define SYSTICK_CVR  (*(volatile uint32_t *)0xE000E018)
#define SYSTICK_ENABLE 1
#define SYSTICK_TICKINT 2
#define SYSTICK_CLKSOURCE 4

void kernel_init(void) {
    scheduler_init();
}

void kernel_start(void) {
    // Configure SysTick
    SYSTICK_RVR = SYSTICK_RELOAD;
    SYSTICK_CVR = 0;
    SYSTICK_CSR = SYSTICK_CLKSOURCE | SYSTICK_TICKINT | SYSTICK_ENABLE;

    // Start scheduling (does not return)
    scheduler_start();
}
