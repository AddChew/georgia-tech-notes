# P2L1

## Process and Process Management

- Process = instance of an executing program, represents the execution state of an active application
- Synonymous with "task" or "job"

### Memory

Heap
- Dynamic allocation of memory

Stack 
- Last in first out
- Primarily used for management of function calls and local variables

Virtual vs Physical Address
- Virtual address do not need to correspond to actual location in physical memory (application level)
- Physical address = location in physical memory (hardware level)
- Page tables = mapping between virtual and physical address (managed by OS)
- Due to insufficient physical memory, parts of virtual address space might not be allocated to physical memory, instead stored on disk (memory swapping managed by OS)
- OS must maintain some sort of information about the process address space (page tables), manage access control to memory.
- Processes can share the same virtual address space but their page tables (mapping between the virtual address and physical address) will be different.

### How does OS know what process is doing

OS maintains information for every process that it manages in Process Control Block (PCB) data structure. PCB is created when process is created. Information in PCB is updated when process state changes.

Consists of components such as:
- Program counter = which step of instruction sequence process is currently in
- Stack pointer = points to top of stack

For frequent state updates, information is saved to CPU register. During context switch (switch control to another process, swapping betwen processes), information from CPU register is saved to PCB. Information from PCB for the other process is loaded to CPU register.

Context switch = switch CPU from context of one process to the context of another

Context switching is expensive because:
- Direct costs: number of cycles to load and store instructions (PCB and CPU register)
- Indirect costs: cold cache (cache misses)

Hot cache = data present in cache

### Process Lifecycle

States: Running, Idle (i.e. Ready state), Waiting, Terminated, New

#### Creation

Process (parent) can create child processes. Fork then exec.

Mechanism for process creation:

Fork
- Copy values from parent PCB into child PCB 
- Child continues execution at instruction after fork (child and parent share same program counter)

Exec
- Replace with child image (i.e. child PCB has own values distinct from parent)
- Load new program and start from first instruction (fresh state, child has own program counter)

In unix, "init" process is regarded as the "parent of all processes". Init is the first process that starts after system boots (PID 1). All other processes is forked from it.

In android, "zygote" is regarded as the "parent of all app processes". Zygote process is forked every time a new process is created.

### Process Scheduling

CPU can only execute ready processes. Multiple ready processes on the ready queue.

CPU scheduler (OS component) determines which one of the currently ready processes will be dispatched to the CPU to start running, and how long it should run for.

Role of OS - strive to minimize time spent on overhead operations below
- Preempt = interupt currently executing process and save its current context
- Schedule = run scheduler to choose next ready process to run
- Dispatch = dispatch process onto CPU and switch into its context

Scheduling Design Decisions
- What are appropriate time slice values? Time slice = time allocated to process on CPU
- Metrics to choose next process to run?

How processes end up on ready queue
- I/O request -> I/O queue -> I/O fulfilled
- time slice expired
- fork child
- interupt

### Inter Process Communication (IPC)

Role of IPC mechanism
- transfer data/information between address spaces
- maintain protection and isolation
- provide flexibilty and performance

Message-passing IPC
- OS provides communication channel like shared buffer
- Processes write (send) and read (receive) messages to/from channel
- Pros: OS manages the channel, provides the standardized same API for read/write
- Cons: overheads, every single piece of information to be passed, have to be copied from address space in first process to channel in OS, and then back to address space of second process

Shared memory IPC
- OS establishes a shared channel and maps it into each process address space (think of it as shared memory)
- Processes directly read/write from this memory
- Pros: OS not involved in communication. no overheads from OS side
- Cons: No longer support standardized API, have to reimplement code for interface