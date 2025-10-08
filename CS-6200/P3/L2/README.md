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
- 