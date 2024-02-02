# Memory Management

## Fixed-Size Memory Pool
Dynamic memory allocation (`malloc`/`free`) is unsuitable for highly resource-constrained, deterministic real-time systems due to fragmentation and unpredictable execution time.

This kernel uses a **Fixed-Size Block Memory Pool**.

### Design
- The user provides a static buffer and block size.
- The kernel divides the buffer into a linked list of free blocks.
- **Allocation (`memory_pool_alloc`)**: Pops the head of the free list. $O(1)$ time complexity.
- **Deallocation (`memory_pool_free`)**: Pushes the block back to the head of the free list. $O(1)$ time complexity.

### Safety
- `memory_pool_free` performs a boundary check to ensure the pointer belongs to the pool.
- It also performs an alignment check based on the block size.
- Traverses the free list defensively during free to prevent double-free corruption. (This check makes freeing $O(N)$ for $N$ free blocks in this implementation, but ensures safety during development).
