# Preemptive Scheduler

## Policy
The kernel employs a **Fixed-Priority Preemptive Scheduler**.
- `NUM_PRIORITIES` defines the priority levels (default 8).
- Priority `0` is the highest priority.
- Priority `NUM_PRIORITIES - 1` is the lowest, reserved for the Idle task.

## Data Structures
- **Ready Queues**: An array of linked lists, one for each priority level.
- **Delayed List**: A single linked list containing all blocked/delayed tasks.

## Tick Interrupt
The Cortex-M `SysTick` timer is configured to generate an interrupt (e.g., every 1 ms). 
1. `SysTick_Handler` calls `scheduler_tick()`.
2. The delayed list is evaluated.
3. If a higher-priority task wakes up, `ICSR` is written to set the `PENDSVSET` bit.

## Context Switch
Context switches do not happen inside `SysTick_Handler` directly. Instead, they are deferred to the `PendSV_Handler`, which runs at the lowest interrupt priority. This prevents disrupting other higher-priority hardware interrupts.

### PendSV Flow
1. Save `r4-r11` to the current task's stack.
2. Save PSP to the current task's TCB.
3. Load the next task's TCB.
4. Load PSP from the new TCB.
5. Restore `r4-r11` from the new task's stack.
6. Execute exception return.
