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

### System call flowchart

Reference: https://gatech.instructure.com/courses/467354/pages/12-system-call-flowchart?module_item_id=5282898

In summary, there is a back and forth between user and kernel mode in terms of control to execute a user process. Interface is via system call.

Synchronous mode: wait until system call is complete

### Hardware cache

User kernel transitions is not cheap. Because context switch will swap the data/address currently in the cache. 

Hot cache = requested data/address is available in cache.

Cold cache = requested data/address is unavailable in cache, have to retrieve from main memory

## OS Services

OS has to provide the following services:
- Process management
- File management
- Device management
- Memory management
- Storage management
- Security
- Scheduling
- ...

OS provide these services via system calls.

### Sample system calls in linux

- Send signal to process: KILL
- Set group identity of process: SETGID
- Mount file system: MOUNT
- Read/write system parameters: SYSCTL

## OS types

### Monolithic OS

Pros
- Everything included
- Inlining, compile time optimization

Cons
- Customization, portability, manageability
- Large, high memory footprint
- Performance

### Modular OS 

More commonly used today.

Uses a module interface layer for compatibility of different implementations of the module.

Pros
- Easier to maintain and upgrade
- Smaller footprint
- Less resource intensive, better performance possibility

Cons
- Have to go through interface layer before implementation layer of service, reduce some opportunity for optimizations and impact performance
- Maintenance can be an issue as modules can be from disparate code bases and might have bugs

### Microkernel

Common in embedded systems.

Only contains the basic primitives in OS level such as address space, threads and IPC (inter process communication).

Everything else such as database, file system, disk driver will run outside kernel level, at user level. This means more user kernel context switching. 

Pros
- Small size, lower overhead and performance
- Easy to test and verify whether working as expected

Cons
- Portability - typically very specialized
- Cost of user kernel crossing
