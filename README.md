# Embedded Operating System Kernel for Resource-Constrained Nodes

A minimal, bare-metal embedded operating system kernel built from scratch for ARM Cortex-M microcontrollers.

## Features
- **Preemptive Scheduler**: Priority-based scheduling using SysTick and PendSV.
- **Synchronization**: Mutexes (with Priority Inheritance to prevent priority inversion) and Counting Semaphores.
- **Deterministic Memory Management**: Fixed-size memory block pool allocator, eliminating `malloc()`/`free()` fragmentation issues.
- **Context Switching**: Highly efficient ARM Cortex-M assembly implementations.
- **Reproducible Build**: CMake-based build system targeting QEMU or physical hardware.

## Project Structure
- `kernel/`: Core OS logic (Scheduler, Task Management, Synchronization, Memory).
- `arch/`: Hardware Abstraction Layer (Context Switching, CPU specifics).
- `startup/`: Minimal ARM Assembly boot code.
- `linker/`: Cortex-M Linker script.
- `applications/`: Demonstrations for Scheduler, Mutex, Semaphore, Priority Inversion, and Memory Pools.

## Requirements
- CMake (>= 3.10)
- `arm-none-eabi-gcc` toolchain
- QEMU (`qemu-system-arm`) for emulation

## Build Instructions
```bash
cmake -B build
cmake --build build
```
This generates binaries inside the `build/` directory for all demonstration applications.

## Emulation (QEMU)
To run the scheduler demo:
```bash
qemu-system-arm -M lm3s6965evb -nographic -kernel build/scheduler_demo
```

## Documentation
- [Architecture](docs/architecture.md)
- [Scheduler Details](docs/scheduler.md)
- [Synchronization (Mutex & Priority Inheritance)](docs/synchronization.md)
- [Memory Management](docs/memory.md)
- [Porting Guide](docs/porting.md)

## Status and Limitations
**IMPLEMENTED**:
- Preemptive Scheduling
- Priority Inheritance
- Fixed-Size Memory Pool
- Cortex-M3 Context Switching

**TESTED / SIMULATED**:
- Due to the current environment not having `arm-none-eabi-gcc` and `qemu-system-arm` available, the binaries have not been actively run. 

**MEASURED**:
- NOT AVAILABLE. Hardware measurement is unavailable. Performance numbers cannot be claimed.

**ALGORITHMIC COMPLEXITY**:
- Context Switch: O(1)
- Task Creation: O(1)
- Memory Allocation: O(1)
- Priority Inversion Resolution (Inheritance boost): O(1)