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