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
- More Memory efficient: shared virtual address space in threads, for multi-process, have to allocate each process its own independent address space.
- Communication: communication between threads is cheaper due to shared memory. On the other hand, inter process communication (IPC) between processes is costly

#### Single CPU

Are threads useful when no of threads > no of CPUs?

- Depends on whether the task are IO bound (i.e. idle time) and the cost of context switching between threads (i.e. context switch time). if t_idle > 2 x t_ctx_switch, performance gains.
- Same concept applies to processes, but context switch for process is more expensive due to the need to to create the page tables (virtual to physical memory mapping) for new process
- Threads share the address space, no need to create new page tables, so cost of context switch is cheaper, i.e. t_ctx_switch_threads << t_ctx_switch_processes
- Hide latency of IO operations

#### Apps and OS

Concurrency!!!

### Thread Mechanisms

#### Issues:
- Race condition; multiple threads operate on the same memory address at the same time

#### Concurrency Control and Coordination

Synchronization Mechanisms:

- Mutual exclusion (mutex): only 1 thread at a time allowed to perform operation. 
- Wait on other threads for specific condition before proceeding - think of it as dependencies, use condition variable to control
- Wake other threads from wait state

#### Thread Creation

Thread Data Structure
- Need a data structure to describe a thread. Store information like thread id, register values, stack etc.

Fork Mechanism
- Takes in two params, proc and args
- Proc = procedure that thread will execute
- Args = arguments for the procedure
- Used to create a thread
- != unix fork used to create new process

Join Mechanism
- Takes in 1 param, thread
- Used to terminate a thread (and retrieve results from child threads)

#### Mutual Exclusion

Mutex
- Lock, used when accessing data/state shared among threads
- Gives a thread exclusive access to the resource and blocks other threads from doing so until it releases the lock
- Termed as "acquire the lock", "acquire the mutex"
- Critical Section = section of code protected by mutex (denoted by curly brackets), i.e. shared variable
- Lock/Unlock API - must explicitly lock mutex when enter critical section, and explicitly unlock mutex when exit critical section

Mutex data structure
- Stores the following information
    - locked?
    - owner
    - blocked threads

