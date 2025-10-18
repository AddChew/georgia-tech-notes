# P3L3: 

## Inter-Process Communication

- OS supported mechanisms for interaction among processes (coordination and communication)
- Message passing
    - i.e. sockets, pipes, message queues
- Memory based
    - i.e. shared memory, memory mapped files
- Higher level semantics
    - mechanism that supports more than simply a channel for two processes to coordinate/communicate with each other, has additional details on protocols
    - i.e. files, remote procedural calls (RPC)
- Synchronization primitives
    - i.e. mutexes

### Message Based IPC

- send/recv messages
- OS creates and maintains a channel (i.e. buffer, FIFO queue) used to pass messages among processes
- OS provides interface to processes, i.e. a port
    - processes send/write messages to a port
    - processes recv/read messages from a port
- OS kernel required to
    - establish communication channel (lives in kernel level)
    - perform each IPC operation
        - send: system call + data copy from process address space to communication channel
        - recv: system call + data copy from communication channel to process address space
    - 1 round of request-response interaction requires 4 x kernel crossings + 4 x data copies
- Pros
    - simplicity: kernel does channel management and synchronization
- Cons
    - overheads due to kernel crossing and data copying in and out of kernel

Forms
- Pipes
    - carry bytes stream between 2 processes
    - i.e. connect output from one process to input of another
- Message queues
    - carry "messages" among processes, messages have to follow a certain format
    - OS management include message priorities, scheduling of message delivery etc
    - APIs
        - Unix: SysV and POSIX
- Sockets
    - send(), recv() == pass message buffers in and out of in kernel communication buffer
    - socket() == create kernel level socket buffer
    - associate necessary kernel level processing (i.e. TCP/IP protocol)
    - if on different machines, channel between process and network device
    - if same machine, bypass full protocol stack, i.e. networking

### Memory based IPC

Shared Memory IPC
- read and write to shared memory region
- OS establishes shared channel/buffer between processes
    - physical pages mapped into virtual address space
    - VA(P1) and VA(P2) map to same physical address
    - VA(P1) != VA(P2)
    - physical memory does not need to be contiguous
- Pros:
    - lower overhead, system calls used only in setup, data copies potentially reduced (but not eliminated)
- Cons:
    - overhead due to mappings between processes virtual address spaces and shared memory pages
    - explicit synchronization of shared memory operations
    - added complexity, developer responsibility to manage shared buffer, communication protocol
- APIs:
    - Unix: SysV, POSIX, memory mapped files
    - Android: Ashmem

### Comparison between Message based and Memory based IPC

- Goal: transfer data from one into target address space
- Message based IPC (Copy):
    - CPU cycles to copy data to/from port
- Memory based IPC (Map)
    - CPU cycles to map physical memory into appropriate virtual address spaces (costly)
    - CPU also used to copy data into channel when necessary
    - One time setup cost, use many times, good payoff
    - No user kernel switches involved
- For large data, t(copy) >> t(map)
- Windows: Local Procedural Call (LPC), if data smaller than threshold, use copy else use map

### SysV API

- "Segments" of shared memory, not necessarily contiguous physical pages
- Shared memory is system wide, system limits on number of segments and total size
- How it works
    1. Create
        - OS allocates the required physical memory and assign unique key to it
        - key used to uniquely identify segment within the OS, any other process can refer to segment using the key
        - API
            - shmget (shmid, size, flag): create or open
            - ftok (pathname, proj_id): same args produce same key
    2. Attach
        - Map virtual address of process to physical address of segment
        - Multiple processes can attach to same segment, share access to same physical page
        - API
            - shmat (shmid, addr, flags)
                - attach shared memory segment into process virtual address space
                - addr = virtual address space to map
                - if addr = NULL, system decide and return arbitrary suitable available address space
                - cast addr to appropriate type
    3. Detach
        - Invalidate address mapping
        - API
            - shmdt (shmid)
    4. Destroy
        - segment only removed when explicitly deleted or reboot
        - attach/detach by different processes multiple times during lifecycle of segment
        - persistent, different from malloc non-shared memory which disappears as soon as process exits
        - API
            - shmctl (shmid, cmd, buf): use IPC_RMID command to destroy

### POSIX API

- Does not use segments, use files instead
- key = file descriptor
- How it works
    - Create
        - API
            - shm_open(): returns file descriptor, in tmpfs
    - Attach/Detach
        - API
            - mmap() and unmmap(): map virtual to physical addresses
    - Destroy
        - API
            - shm_close(), shm_unlink()

### Shared Memory Synchronization

- 
