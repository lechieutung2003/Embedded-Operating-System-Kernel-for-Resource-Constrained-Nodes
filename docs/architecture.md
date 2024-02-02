# Kernel Architecture

## Overview
This operating system kernel is designed for resource-constrained ARM Cortex-M microcontrollers. It provides deterministic execution, minimal overhead, and essential real-time operating system (RTOS) features.

## Layered Architecture
```
+-------------------------------------------------------+
|                 Application Tasks                     |
+-------------------------------------------------------+
|                   Kernel API                          |
| (task_create, mutex_lock, semaphore_wait, etc.)       |
+-------------------------------------------------------+
|  Scheduler | Synchronization | Memory Manager         |
| (Ready Qs) | (Mutex, Sem)    | (Fixed-size Pool)      |
+-------------------------------------------------------+
|                 Context Switching                     |
|                 (PendSV, SysTick)                     |
+-------------------------------------------------------+
|            ARM Cortex-M Hardware Layer                |
|              (NVIC, Registers, CPU)                   |
+-------------------------------------------------------+
```

## Core Components

### 1. Hardware Abstraction
The `arch/arm/` directory contains all hardware-specific code. 
- `context_switch.s`: Assembly for PendSV-based context switching.
- `cpu.c`: Critical section management using `PRIMASK`.
- `exceptions.c`: Definitions for SysTick and Fault handlers.

### 2. Scheduler
A fixed-priority preemptive scheduler. Handles task state transitions and scheduling decisions.

### 3. Task Model
Tasks are represented by a Task Control Block (TCB). Stacks are statically allocated by the application to avoid dynamic memory allocation overhead and fragmentation.

### 4. Synchronization
- **Mutex**: Includes a built-in Priority Inheritance mechanism to prevent priority inversion.
- **Semaphore**: Counting semaphore for producer/consumer signaling.

### 5. Memory Management
A deterministic $O(1)$ fixed-size memory pool allocator. Eliminates the need for standard `malloc()`/`free()`.
