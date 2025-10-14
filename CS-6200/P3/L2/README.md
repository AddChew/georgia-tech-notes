# P3L2: 

## Memory Management

- virtual memory >> physical memory
- excess spillover in virtual memory not present in physical memory, is stored in secondary storage (i.e. disk)
- memory swapping: OS must have mechanism to swap contents in physical memory with needed content from temporary storage (i.e. disk)
- arbitrate: mapping between virtual and physical memory via page tables + validate legal memory access

Page based memory management (More commonly used)
- virtual memory: unit = page (fixed size)
- physical memory: unit = page frame (fixed size)
- mapping: page tables

Segment based memory management
- virtual/physical memory: unit = segment (flexible size)
- mapping: segment registers

Hardware support
- memory management unit (MMU): 
    - translate virtual to physical addresses
    - report faults: illegal access, permission, not present in memory must fetch from disk
- registers
    - pointers to page table
    - base, limit size, number of segments
- cache - translation lookaside buffer (TLB)
    - cache valid virtual address to physical address translation
    - if translation in cache, can skip operations to access page table

actual translation performed by hardware

Page tables
- created per process, on context switch, switch to valid page table
- map virtual to physical address
- virtual memory page size == physical memory page frame size
- Only need to keep track first virtual address mapping to first physical address, the rest will map to the corresponding offsets
- Virtual address data structure stores both the virtual page number (VPN) and offset
- Physical address data structure stores physical frame number (PFN), offset and valid/present bit (indicates whether page is in memory, 0 = no, 1 = yes)

Page table entry
- flags:
    - present bit: valid, invalid 
    - dirty bit: set whenever a page is written to
    - accessed: for read or write
    - r/w permission bit: read and/or write access permissions (0 = read only, 1 = read and write)
    - u/s permission bit: 0 = user mode, 1 = supervisor mode only
    - others: caching related info (write through, caching disabled)
    - unused: for future
- if hardware determines that physical memory access cannot be performed, it causes a page fault, CPU will place error code on stack of kernel, generate trap into OS kernel, generate page fault handler which determines action based on error code and fault address

Page table size
- process does not use entire address space
- but page table assumes an entry per VPN, whether corresponding virtual memory is needed or not

Multi level/Hierarchical page tables
- consist of outer/top page table (page table directory) and internal page table (only for valid virtual memory regions)
- extra index in virtual address to index into different levels of page table hierarchy
- pros:
    - reduced page table size, smaller internal page tables/directories granularity of coverage
- cons:
    - more memory accesses for translation, resulting in increased translation latency

Page table cache
- translation lookaside buffer (TLB) 
- solution for slow memory access in multi level page tables translation
- check cache, TLB hit, use cache results, bypass the required memory accesses to perform translation

Inverted page tables
- Map in the opposite direction, from physical to virtual memory
- 1 entry for each element of physical memory
- limitations:
    - have to perform linear search of page table to locate entry that matches process pid and virtual address
- workarounds:
    - translation lookaside buffer, reduce occurence of linear search
    - hashing page tables:
        - hash function, compute hash on part of address, hash is entry in hash table
        - hash table points to linked list of possible matches for this part of address
        - narrow the search space

Segmentation
- segments = arbitrary granularity (logically meaningful component), i.e. code, heap, data, stack
- address = segment selector + offset
- segment = contiguous physical memory
- segment size = segment base + limit registers
- in practice, segmentation + paging are used together

Page size
- Larger pages:
    - pros: fewer page table entries, smaller page tables, more TLB hits
    - cons: internal fragmentation, wasted memory due to unused space, i.e. assigned memory space > used memory space

Memory allocation
- role of memory allocator
    - determines virtual address (VA) to physical address (PA) mapping
- kernel level allocator
    - allocate memory region for kernel
    - kernel state, static process state
- user level allocator
    - used for dynamic process state (heap), i.e. malloc, free
- challenges
    - external fragmentation, there is sufficient memory available, but they are non-contiguous, hence cannot be used

Linux kernel allocator
- buddy allocator
    - start with 2^x area
    - on request, subdivide into 2^x chunks and find smallest 2^x chunk that can satisfy request
    - fragmentation still present
    - on free, check buddy to see if can aggregate into larger chunk (mitigate external fragmentation)
    - pros: aggregation works well and fast
    - cons: internal fragmentation due to 2^x granularity
- slab allocator
    - caches for common object types/sizes, on top of contiguous memory
    - pros: 
        - avoids internal fragmentation, slab is exact same size as kernel objects
        - external fragmentation not an issue

Demand paging
- 