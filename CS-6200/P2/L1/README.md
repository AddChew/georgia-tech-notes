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
