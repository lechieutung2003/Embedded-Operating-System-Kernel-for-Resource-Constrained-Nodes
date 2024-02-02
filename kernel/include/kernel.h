#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stddef.h>

#define MAX_TASKS 16
#define NUM_PRIORITIES 8 // 0 is highest, 7 is lowest

typedef enum {
    KERNEL_OK = 0,
    KERNEL_ERROR,
    KERNEL_INVALID_ARGUMENT,
    KERNEL_OUT_OF_MEMORY,
    KERNEL_TIMEOUT,
    KERNEL_RESOURCE_BUSY
} kernel_status_t;

void kernel_init(void);
void kernel_start(void);

// Critical sections
void critical_section_enter(void);
void critical_section_exit(void);

#endif // KERNEL_H
