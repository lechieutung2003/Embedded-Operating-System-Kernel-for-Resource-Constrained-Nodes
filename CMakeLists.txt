cmake_minimum_required(VERSION 3.10)
project(EmbeddedOS C ASM)

# Toolchain configuration (assuming arm-none-eabi-gcc is in PATH when the user runs this)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

set(CPU_FLAGS "-mcpu=cortex-m3 -mthumb -mfloat-abi=soft")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CPU_FLAGS} -Wall -Wextra -g -O0 -ffunction-sections -fdata-sections")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${CPU_FLAGS} -g")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T${CMAKE_SOURCE_DIR}/linker/linker.ld -Wl,--gc-sections --specs=nosys.specs")

include_directories(
    kernel/include
    arch/arm
)

set(KERNEL_SOURCES
    kernel/src/kernel.c
    kernel/src/task.c
    kernel/src/scheduler.c
    kernel/src/mutex.c
    kernel/src/semaphore.c
    kernel/src/memory_pool.c
)

set(ARCH_SOURCES
    arch/arm/context_switch.s
    arch/arm/exceptions.c
    arch/arm/cpu.c
)

set(STARTUP_SOURCES
    startup/startup.s
)

# Demo: Scheduler
add_executable(scheduler_demo 
    applications/scheduler_demo/main.c
    ${KERNEL_SOURCES}
    ${ARCH_SOURCES}
    ${STARTUP_SOURCES}
)

# Demo: Mutex
add_executable(mutex_demo 
    applications/mutex_demo/main.c
    ${KERNEL_SOURCES}
    ${ARCH_SOURCES}
    ${STARTUP_SOURCES}
)

# Demo: Semaphore
add_executable(semaphore_demo 
    applications/semaphore_demo/main.c
    ${KERNEL_SOURCES}
    ${ARCH_SOURCES}
    ${STARTUP_SOURCES}
)

# Demo: Priority Inversion
add_executable(priority_inversion_demo 
    applications/priority_inversion_demo/main.c
    ${KERNEL_SOURCES}
    ${ARCH_SOURCES}
    ${STARTUP_SOURCES}
)

# Demo: Memory Pool
add_executable(memory_pool_demo 
    applications/memory_pool_demo/main.c
    ${KERNEL_SOURCES}
    ${ARCH_SOURCES}
    ${STARTUP_SOURCES}
)
