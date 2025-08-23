# P1L2

## Operating System Definition

Operating system = software that abstracts (simplifies) and arbitrates (manages) the hardware system

Specifically it:
- Has priviledged access to underlying hardware
- Hide hardware complexity
- Manages hardware on behalf of one or more applications according to some predefined policies
- Ensures that applications are isolated and protected from one another

Cache memory is not a component of an operating system as OS does not directly manage the cache; hardware manages cache itself.

## Operation System Examples

Desktop
- Microsoft Windows
- UNIX based
    - Mac OS X (BSD)
    - Linux

Embedded
- Android
- iOS
- Symbian

## OS Elements

Each OS element consists of abstractions, mechanisms and policies.

Abstractions
- processes, threads, files, sockets, memory pages

Mechanisms
- create, schedule, open, write, allocate

Policies
- max sockets per process, least recently used (LRU) algorithm to move data from DRAM (physical memory) to disk

### Memory Management Example

Abstractions
- Memory page

Mechanisms
- allocate, map to a process (allow process to access memory corresponding to page)

Policies
- least recently used (LRU) - decide whether to store data on memory (DRAM) or disk (memory swapping)

## OS Design Principles

### Separation of Mechanism and Policy

- Implement flexible mechanisms to support many polices
- i.e. LRU, LFU, random

### Optimize for common use case

- Where will the OS be used?
- What will user want to execute on that machine?
- What are the workload requirents?

Based on use case, pick the most appropriate policy.

## OS Protection Boundary

### User (Unprivileged) vs Kernel (Privileged) mode

In general, applications operate on user level while OS operate on kernel level - direct access to hardware. 

This is controlled via a switch - bit set on CPU, if in kernel mode, bit is set on CPU, allow direct access to hardware. In user mode, bit not set on CPI, cannot access hardware directly.

Trying to access hardware directly in user mode will result in a trap; application will be interupted and hardware will switch control to OS. OS will check what caused the trap, whether operation should be allowed to continue or terminated.

Interaction between OS and application is via system call interface - think of system call as the language used by application to communicate to OS.

OS support signals - mechanism for OS to notify application