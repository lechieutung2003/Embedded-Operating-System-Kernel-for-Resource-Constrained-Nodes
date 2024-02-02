#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"

void scheduler_init(void);
void scheduler_start(void);
void scheduler_tick(void);

void scheduler_add_ready(tcb_t *task);
void scheduler_remove_ready(tcb_t *task);
void scheduler_block_task(tcb_t *task);
void scheduler_unblock_task(tcb_t *task);

tcb_t* scheduler_get_current_task(void);
tcb_t* scheduler_get_next_task(void);

void scheduler_request_context_switch(void);

#endif // SCHEDULER_H
