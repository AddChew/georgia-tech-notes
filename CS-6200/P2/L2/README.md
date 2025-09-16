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

#### Condition Variable

Mechanism:

Wait
- Take in 2 params, mutex and condition variable
- mutex is automatically released and reacquired on wait

Signal
- Takes in 1 param, condition
- notify only 1 thread waiting on condition

Broadcast
- Takes in 1 param, condition
- notify all waiting threads

Condition Variable data structure
- Stores the following information
    - mutex reference
    - waiting threads

##### Reader Writer Problem

Problem:
- Issue in databases
- Allow 0 or more readers to access the shared resource concurrently
- Allow 0 or 1 writer to access the shared resource
- Mutex lock is too restrictive for reader access.

Solution:

Use a proxy resource (helper variable)
- State of shared resource
    - free: resource counter = 0
    - read: resource counter > 0 = number of readers
    - write: resource counter = -1 

Use mutex to control access to this proxy resource instead of access to the shared resource

##### Critical Section

Typical structure (No Proxy Variable)
```
Lock(mutex){
    while(!predicate_indicating_access_ok)
        wait(mutex, cond_var)
    update state => update predicate
    signal and/or broadcast(cond_var with correct waiting threads)
} // unlock
```

Critical Section Structure with Proxy Variable

Enter critical section
```
Lock(mutex) {
    while(!predicate_for_access)
        wait(mutex, cond_var)
    update predicate
} // unlock
```

Exit critical section
```
Lock(mutex) {
    update predicate
    signal/broadcast(cond_var)
} // unlock
```

#### Pitfalls 

##### Good Practices

- Keep track of the mutex/cond variables used for each resource (i.e. leave a comment to state this mutex/cond variable used to protect which resource)
- Ensure always lock and unlock
- Use a single mutex to access a single resource
- Ensure signal correct condition
- Ensure that you are not using signal when broadcast is needed
    - signal: only 1 thread will proceed, the rest will continue to wait ... possibly indefinitely, result in a deadlock scenario

##### Spurious Wake-Ups

- Unnecessary wake up to waiting threads
- No impact to correctness but impact to performance
- Happens when we structure the signal/broadcast within the lock block. During the signal/broadcast, possible that no threads can actually get the lock and do something as the lock might still be unreleased. Threads moved out of waiting queue before lock released.
- Wasteful due to the context switches
- Workaround is to put the signal/broadcast outside after the lock. But this does not work all the time.

Replace
```
lock(counter_mutex) {
    resource_counter = 0;
    broadcast(read_phase);
    signal(write_phase);
} // unlock
```

With
```
lock(counter_mutex) {
    resource_counter = 0;
} // unlock
broadcast(read_phase);
signal(write_phase);
```

##### Deadlock

2 or more competing threads are waiting on each other to complete but none of them ever do

###### Example

- Need both lock A and B to proceed. T1 has lock A and is waiting to get lock B. T2 has lock B and is waiting to get lock A. Results in an endless cycle.

Solution
1. Fine-grained locking: unlock A before lock B, basically at any point in time, only one can be locked (Does not work in the example since we need both)
2. Get all the locks upfront, then release at the end (Too restrictive, limits parallelism)
3. Maintain lock order (preferred approach)
    - Must obtain lock A first
    - then lock B

Cycle = deadlock

What can we do about deadlocks?
- Deadlock prevention - expensive
- Deadlock detection and recovery - rollback
- Do nothing - if fail, just reboot

#### Kernel vs User level threads

##### Models

1 to 1 Model
- Each user level thread has a kernel level thread associated with it
- Pros: 
    - OS understands threads,sychronization, blocking etc
- Cons:
    - Must go to OS for all operations - expensive
    - OS may have limits on policies, thread number etc
    - Portability, need to find a specific kernel that supports your specific thread management policy

Many to 1 Model
- Multiple user level threads associated to 1 kernel level thread
- Thread management library on user level that decides which user level thread is mapped to kernel level thread at any point in time
- Pros:
    - Portable, everything done on user level, dont depend on OS limits and policies
- Cons:
    - OS has no visibility to application needs
    - OS might block entire process if 1 user level thread blocks on IO

Many to Many Model
- Some user level threads use the 1 to 1 Model
- Other user level threads use the Many to 1 Model
- Pros:
    - best of both worlds
    - can have bound or unbound threads
- Cons:
    - requires coordination between user and kernel level thread managers

##### Multithreading Scope

Kernel level (System scope)
- System wide thread management
- Managed by OS level thread managers

User level (Process scope)
- User level library that manages threads within a single process
- Different processes managed by different instance of same library

#### Multithreading Patterns

##### Boss Workers

- Boss thread: assign work to workers
- Worker thread: perform entire task
- Throughput of the system is limited by boss thread, must keep boss efficient
- Throughput = 1 / boss time per order

Pattern 1:
- Boss assigns work by directly signalling specific worker
- Pros:
    - Workers dont need to synchronize
- Cons
    - Boss must track what each worker is doing
    - Thoughput will go down

Pattern 2 (Preferred):
- Boss assigns work by placing work in producer consumer queue    
- Pros:
    - Boss dont need to know details about worker
- Cons
    - Queue synchronization, thread pool management
    - Locality, might be more efficient to assign same type of task to same worker, but not possible since boss does not know what worker is doing
- Worker pool can be static or dynamic
- Workaround for locality issue is to have workers specialized for certain tasks instead of all workers created equal
    - better locality
    - better quality of service management, assign more threads to tasks that require higher quality of service
    - Cons:
        - load balancing, how many threads to assign for the different tasks?

##### Pipeline

- threads assigned subtask in system
- entire task executed as pipeline of threads
- multiple tasks concurrently in the system in different pipeline stages
- each stage = thread pool
- throughput = weakest link
    - assign more threads to botteneck stage to balance it out
- shared buffer based communication between stages
- Pros:
    - specialization and locality
- Cons:
    - balancing and synchronization overheads

##### Layered

- each layer is a group of related subtasks
- end to end task must pass up and down through all layers
- Pros:
    - specialization
    - less fine grained than pipeline
- Cons
    - not suitable for all applications
    - synchronization