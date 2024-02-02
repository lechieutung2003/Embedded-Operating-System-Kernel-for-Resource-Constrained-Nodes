#include "kernel.h"
#include "task.h"
#include "mutex.h"

// Priority Inversion Scenario
// Task L (Low priority) locks Mutex.
// Task H (High priority) tries to lock Mutex and blocks.
// Task M (Medium priority) starts running and preempts Task L.
// Without priority inheritance, Task H would wait for Task M to finish.
// With priority inheritance, Task L inherits Task H's priority, preempts Task M, releases Mutex, and Task H runs.

mutex_t resource_mutex;

static tcb_t task_h_tcb;
static uint32_t task_h_stack[256];

static tcb_t task_m_tcb;
static uint32_t task_m_stack[256];

static tcb_t task_l_tcb;
static uint32_t task_l_stack[256];

volatile uint32_t trace_buffer[64];
volatile uint32_t trace_idx = 0;

void log_trace(uint32_t event) {
    if (trace_idx < 64) {
        trace_buffer[trace_idx++] = event;
    }
}

void task_l_entry(void *arg) {
    (void)arg;
    log_trace(1); // Task L starts
    mutex_lock(&resource_mutex);
    log_trace(2); // Task L gets mutex
    
    // Simulate work that gets interrupted
    for(volatile int i=0; i<10000; i++); 

    log_trace(3); // Task L finishes work
    mutex_unlock(&resource_mutex);
    log_trace(4); // Task L unlocks
    while(1) task_delay(100);
}

void task_m_entry(void *arg) {
    (void)arg;
    task_delay(5); // Start later than L
    log_trace(5); // Task M starts, preempting L if no inheritance
    for(volatile int i=0; i<50000; i++); // Long work
    log_trace(6); // Task M ends
    while(1) task_delay(100);
}

void task_h_entry(void *arg) {
    (void)arg;
    task_delay(10); // Start after M
    log_trace(7); // Task H starts
    mutex_lock(&resource_mutex);
    log_trace(8); // Task H gets mutex
    mutex_unlock(&resource_mutex);
    log_trace(9); // Task H ends
    while(1) task_delay(100);
}

int main(void) {
    kernel_init();
    mutex_init(&resource_mutex);

    // Priority 1 is highest here, 3 is lowest.
    task_create(&task_h_tcb, task_h_entry, NULL, 1, task_h_stack, sizeof(task_h_stack));
    task_create(&task_m_tcb, task_m_entry, NULL, 2, task_m_stack, sizeof(task_m_stack));
    task_create(&task_l_tcb, task_l_entry, NULL, 3, task_l_stack, sizeof(task_l_stack));

    kernel_start();
    return 0;
}
