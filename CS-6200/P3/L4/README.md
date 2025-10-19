# P3L4:

## Synchronization Constructs

Methods
1. Repeatedly check whether to continue
    - Sync via spinlocks
2. Wait for signal to continue
    - Sync using mutexes and condition variables
 
Waiting hurts performance: CPUs waste cycles for checking, cache effects

### Limitations of Mutexes/Condition Variables

- error prone/correctness/ease of use
    - unlock wrong mutex, signal wrong condition variable
- lack of expressive power
    - need helper variables for access/priority control

### Spinlocks

- Similar to mutex
    - mutual exclusion
    - lock and unlock (free)
- Difference
    - in spinlock, when lock is busy, thread will spin, continually running on CPU and repeatedly check if lock is free, until lock becomes free or thread gets preempted
    - in mutex, when lock is busy, thread will release CPU, allow another thread to run

Code snippet
```C
spinlock_lock(s);
    // critical section
spinlock_unlock(s);
```

### Semaphores

- Represented by integer value
- On init
    - assigned a max value, some positive integer
    - if initialized with 1, binary semaphore, behaviour similar to mutex
- On try (wait)
    - if non-zero, decrement and proceed
    - if zero, wait
    - number of threads that can proceed = maximum value used when semaphore was initialized
- On exit (post)
    - increment semaphore counter
- Pros
    - express count related synchronization requirements

Code Snippet
```C
#include <semaphore.h>

sem_t sem;
sem_init(sem_t *sem, int pshared, int count); // pshared flag indicates whether semaphore is shared by threads in single process or across processes
sem_wait(sem_t *sem);
sem_post(sem_t *sem);
```

### Reader/Writer Locks

- Read
    - never modify
    - shared access/locks
- Write
    - always modify
    - exclusive access/locks
- Reader/Writer Locks
    - define the type of access, lock behaves accordingly
- Semantic differences across implementations
    - recursive read lock
        - in some implementations, single read unlock may unlock every single read_lock that have been recursively invoked from within same thread
        - other implementations, separate read_unlock required for every single read_lock
    - upgrade/downgrade priority
        - in some implementations, reader (owner of shared lock) given priority to upgrade to writer lock compared to newly arriving request for write lock
        - other implementations, owner of read lock will first release it and then try to reacquire it with write permissions, have to contend with other threads trying to perform same operation
    - interaction with scheduling policy
        - i.e. block reader if higher priority writer waiting

Code Snippet
```C
#include <linux/spinlock.h>
rwlock_t m;
read_lock(m);
    // critical section
read_unlock(m);

write_lock(m);
    // critical section
write_unlock(m);
```

### Monitors

- Higher level synchronization construct
- Specify
    - shared resource
    - entry procedure
    - possible condition variables
- On entry
    - lock, check, ... happen automatically
- On exit
    - unlock, check, signal ... happen automatically

### Atomic Instructions

Critical section with hardware supported synchronization

Hardware specific
- test_and_set
- read_and_increment
- compare_and_swap

Guarantees
- atomicity
- mutual exclusion
- queue all concurrent instructions but one

test_and_set(lock)

```C
spin_lock(lock): //spin until free
    while(test_and_set(lock) == busy);
```

- atomically returns tests original value and sets new value = 1 (busy)
- first thread: test_and_set(lock) == 0, lock = free
- next threads: test_and_set(lock) == 1, lock = busy

### Shared Memory Multiprocessors (Symmetric Multiprocessors - SMPs)

Can be bus-based or interconnect (I/C) based
- expensive due to bus or I/C contention
- expensive due to cache bypass and coherrence traffic

Caches
- CPU layer, each CPU has own cache
- Hide memory latency, memory "further away" due to contention (i.e. multiple CPUs have access and can modify it)
- no-write (write to memory only without writing to cache), write-through (write to both memory and cache), write-back (write to cache, write to memory is delayed, performed later)

Cache Coherence
- non-cache-coherrent (NCC): hardware does not synchronize cache values across CPUs automatically, have to be handled by software
- cache-coherent (CC): hardware will take care of synchronizing cache values across CPUS automatically
- write-invalidate (WI)
    - hardware will invalidate cache value in other CPUs if it is modified, when other CPUs try to reference cache value, will result in cache miss, fetch from memory
    - pros:
        - lower bandwidth, amortize cost
- write-update (WU): 
    - hardware will update cache value in other CPUs when it is modified
    - pros
        - update available immediately
- Atomics always issued to memory controller, bypass cache
    - pros
        - central entry point, references can be ordered and synchronized, remove race conditions
    - cons
        - takes much longer, generates coherence traffic regardless of change

### Spinlock

#### Performance Metrics

- Reduce latency
    - reduce time taken for thread to acquire a free lock
    - ideally immediately execute single atomic instruction
- Reduce wait time (delay)
    - reduce time to stop spinning and acquire free lock (time to acquire free lock after waiting)
    - ideally immediately
- Reduce contention
    - reduce bus/network/i-c traffic (requests)
    - contention due to 
        - atomic memory reference
        - coherrence traffic
    - ideally 0

#### Test and Set Spinlock

Implementation 1: Test and Set Spinlock

```C
spinlock_init(lock):
    lock = free; // 0 = free, 1 = busy

spinlock_lock(lock): // spin
    while(test_and_set(lock) == busy);
    
spinlock_unlock(lock):
    lock = free;
```

Pros
- Minimal latency: just atomic
- Minimal delay: continuously spinning on atomic

Cons
- Contention: processors go to memory on each spin

Implementation 2: Test and Test and Set Spinlock (Spin on Read/Spin on cache value)

```C
// test using cache, only if cache lock free then execute atomic test_and_set to potentially reduce contention
// spin on cache lock == busy
// atomic if freed, test_and_set
spinlock_lock(lock):
    while((lock == busy) OR test_and_set(lock) == busy);
```

Pros
- Slighly more overhead than test_and_set due to additional check, but acceptable latency and delay

Cons
- Non-cache-coherrent: no difference in contention as still have to access memory to retrieve cache value
- Cache-coherrent + Write Update: OK
- Cache-coherrent + Write Invalidate: Horrible, contention due to atomics, cache invalidated, have to access memory to fetch lock value due to cache miss, more contention

#### Spinlock "Delay" Alternatives

```C
spinlock_lock(lock):
    while((lock == busy) OR test_and_set(lock) == busy) {
        // failed to get lock
        while(lock == busy);
        delay();
    }
```

Everyone sees lock is free but not everyone attempts to acquire it

Pros
- Improvement in contention
- Latency OK

Cons
- Delay much worse

Alternative Delay Implementation

```C
spinlock_lock(lock):
    while((lock == busy) OR test_and_set(lock) == busy) {
        delay();
    }
```

Delay after each lock reference
- does not spin continuously
- works on non-cache coherrent hardware
- hurts delay even more

Picking a Delay
- Static delay
    - based on fixed value, i.e. CPU ID
    - simple approach
    - unnecessary delay under low contention
- Dynamic delay
    - backoff-based
    - random delay in a range that increases with "perceived" contention
    - "perceived" == failed test_and_set()
    - problem: delay after each reference will keep growing based on contention or length of critical section, need to be able to guard against this case where high failed test_and_set() cases is not due to high contention but rather long execution time of critical section (Don't need to bump up delay in this case)

#### Queuing Lock

- Seeks to solve the problem where everyone sees the lock is free at same time, if not everyone sees the lock is free at same time, then not everyone will try to acquire the lock at same time
- Uses an array of "flags" with N elements
    - N = number of processes
    - every element has either of two states, has lock or must wait
    - pointer to current lock holder
    - pointer to last element in queue
    - each arriving thread assigned a queue[ticket] == private lock
        - queue[ticket] == must wait, spin
        - queue[ticket] == has lock, enter critical section
        - signal/set next lock holder on exit, queue[ticket + 1] = has lock
- Cons
    - assumes hardware support for read_and_increment atomic
    - O(N) size for memory