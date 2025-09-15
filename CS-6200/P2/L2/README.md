# P2L2

## Threads and Concurrency

### Thread
- 1 process can have many threads executing concurrently
- Executing unit of a process
- Requires coordination between threads due to sharing of resources (i.e. I/O devices, CPUs and memory)
- Thread execute same code, but not the same instructions - need to have own (data structure) copy of register, stack etc
- PCB will store shared information among threads (i.e. code, data, files) + per thread execution context

### Why Threads are Useful?

#### Benefits
- Parallelization: speed up
- Specialization - doing same task type: reuse hot cache, performance gains
- More Memory efficient: shared memory address space in threads, for multi-process, have to allocate each process its own independent address space.
- Communication: communication between threads is cheaper due to shared memory. On the other hand, inter process communication (IPC) between processes is costly

#### Single CPU