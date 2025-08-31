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
