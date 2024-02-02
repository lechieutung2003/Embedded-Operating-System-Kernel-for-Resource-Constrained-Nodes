# Synchronization

## Mutex and Priority Inheritance

### Priority Inversion Problem
If a low-priority task (L) holds a mutex required by a high-priority task (H), H blocks. If a medium-priority task (M) preempts L, H is indirectly blocked by M.

### Solution: Priority Inheritance
When H attempts to lock a mutex held by L:
1. The kernel detects that L's priority is lower than H's.
2. L's priority is temporarily boosted to match H.
3. L is re-queued in the Ready Queue at the new higher priority.
4. L preempts M, finishes its critical section, and unlocks the mutex.
5. L's priority is restored to its base priority.
6. H acquires the mutex and runs.

## Counting Semaphores
Semaphores are used for signaling (e.g., Interrupt to Task, or Task to Task).
- `semaphore_wait()`: Decrements the count. Blocks if count is 0.
- `semaphore_post()`: Increments the count. Unblocks the first waiting task.
