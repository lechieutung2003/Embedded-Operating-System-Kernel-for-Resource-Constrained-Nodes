#include "scheduler.h"

// Define SysTick_Handler
void SysTick_Handler(void) {
    scheduler_tick();
}

// Exception handlers for faults
void HardFault_Handler(void) {
    while(1);
}

void MemManage_Handler(void) {
    while(1);
}

void BusFault_Handler(void) {
    while(1);
}

void UsageFault_Handler(void) {
    while(1);
}

void SVC_Handler(void) {
    // SVCall used for kernel calls if we were running in unprivileged mode
    // Since we are in bare-metal privileged, we might not strictly need SVC.
}
