# P3L6: Virtualization

Virtualization allows concurrent execution of multiple OSs (and their applications) on the same physical machine

Virtual resources = each OS thinks that it "owns" the underlying hardware resources

Virtual machine (VM) = OS + applications + virtual resources (guest domain)

Virtualization layer = management of physical hardware (virtual machine monitor, hypervisor)

## Classical Definition

A virtual machine is an efficient, isolated duplicate of the real machine

Supported by a virtual machine monitor (VMM)
- fidelity: provides environment identical with original machine
- performance: programs in virtual machine show at worst only minor decrease in speed (goal is for VM to perform as close to native performance as possible)
- safety and isolation: VMM is in complete control of system resources

## Benefits

- Consolidation: ability to run multiple virtual machines (with different OS and applications) on a single physical platform
    - decrease cost, improve manageability
- Migration: easy to migrate OS in applications to other machines (easy to scale) as not coupled to physical hardware
    - portability, availability, reliability
- Security: more easy to contain bugs to resources available to virtual machine only, dont affect entire hardware system
- Debugging
- Support for legacy OSs

## Virtualization Models

### Bare-Metal (Hypervisor-Based) - Type 1

- VMM (hypervisor) manages all hardware resources and supports execution of VMs
- Issue for this model is that VMM have to manage all devices:
    - Solution: integrate a special virtual machine (privileged service VM) to deal with devices (and other configuration and management task)

Xen (open source or Citrix XenServer)
- dom0: privilege domain, drivers in dom0
- domU: guest VMs
- Xen: hypervisor

ESX (VMware)
- drivers in VMM
- used to have Linux control core, now remote API

### Hosted - Type 2

- host OS owns all hardware
- special VMM module provides hardware interfaces to VMs and deals with VM context switching
- pros: can leverage all of the services and mechanisms on host OS, less functionality needs to be redeveloped for VMM module

Example: KVM (Kernel-based VM)
- based on Linux
- KVM kernel module + QEMU (hardware emulator) for hardware virtualization
- pros: leverage linux open source community

## Hardware Protection Levels

Commodity hardware has more than 2 protection levels

x86 has 4 protection levels (rings) and 2 protection modes (root and non-root) - case without VM
- ring 3: lowest privilege (apps)
- ring 0: highest privilege (OS)

Case with VM
- non-root: VMs
    - ring 3: lowest privilege (apps)
    - ring 0: OS

- root:
    - ring 0: hypervisor

Attempt by guest OS to perform privileged operations cause traps called `VMexits`. Trigger a switch to root mode and pass control to hypervisor. When hypervisor complete operation, pass control back to VM via VMentry, switch to non-root mode

## Processor Virtualization (Trap and Emulate)

Guest instructions
- executed directly by hardware
- for non-privileged operations: hardware speeds -> efficiency
- for privileged operations: trap to hypervisor
- hypervisor determines what needs to be done:
    - if illegal op: terminate VM
    - if legal op: emulate behaviour guest OS was expecting from hardware

Limitations
- x86, pre 2005
    - only 4 rings, no root/non-root modes yet
    - hypervisor in ring 0, guest OS in ring 1
    - but 17 privilege instructions not trapped, fail silently!
    - hypervisor doesnt know, it doesnt try to change settings
    - guest OS doesnt know, so assume change successful

Solutions
1. Binary translation
    - rewrite VM binary to never issue those 17 instructions
    - goal: full virtualizationn = guest OS not modified (no installation of special drivers, policies etc to change guest OS in order to run it in virtual environment)
    - approach: dynamic binary translation
        1. inspect code blocks to be executed, check if any of 17 instructions is about to be issued
        2. if no 17 instructions, mark as safe, allow it to run natively at hardware speeds
        3. if bad instructions found, translate them into some other instruction sequence that avoids the undesired instruction and emulates the desired behaviour
    - efficiency improved via mechanisms like
        - cache translated code blocks to amortize translation costs
2. Paravirtualization
    - goal: performance; give up on unmodified guests
    - approach: paravirtualization = modify guest so that
        - it knows its running virtualized
        - it makes explicit calls to hypervisor (hypercalls)
        - hypercall similar to system call behaviour
            - package context info
            - specify desired hypercall
            - trap to VMM

## Memory Virtualization

Full Virtualization
- all guests expect contiguous physical memory, starting at 0
- virtual (used by apps in guest) vs physical (guest thinks are physical addresses of resources) vs machine addresses (actual physical address on hardware) and page frame numbers

Option 1
- guest page table: VA -> PA
- hypervisor: PA -> MA
- too expensive! 

Option 2
- guest page table: VA -> PA
- hypervisor shadow page table: VA -> MA
- hypervisor maintains consistence

Paravirtualization
- guest aware of virtualization
- no strict requirement on contiguous physical memory starting at 0
- explicitly register page tables with hypervisor
- "batch" page tables updates to reduce VM exits

## Device Virtualization

Passthrough model
- approach: VMM level driver configures device access permissions
- pros: VM provided with exclusive access to the device; VM can directly access the device (VMM - bypass)
- cons: 
    - device sharing difficult
    - VMM must have exact type of device as what VM expects; guest VM and device driver in guest VM directly access device; guest vm device driver must be compatible with device
    - VM migration tricky, VM and hardware no longer decoupled

Hypervisor - Direct model
- approach: VMM intercepts all device accesses
- emulate device operation
    - translate to generic I/O operation
    - transverse VMM-resident I/O stack
    - invoke VMM-resident driver
- pros:
    - VM decoupled from physical device
    - sharing, migration, dealing with device specifics simpler
- cons:
    - latency of device operations
    - device driver ecosystem complexities in hypervisor

Split-Device Driver model
- approach: device access control split between
    - front end driver in guest VM
    - back end driver in service VM (or host) hypervisor layer
- modified guest VM driver: create messages that get passed to service VM, will not try to access the device directly (limited to paravirtualized guests)
- pros: 
    - eliminate emulation overhead
    - allow more better management of shared devices

## Hardware Virtualization

Improvements to x86 after 2005
- "close holes" in x86 ISA in terms of 17 non-virtualizable instructions
- modes: root/non-root (or host and guest mode)
- VM control structure
    - VCPU - support added for hardware processor to understand and interpret info that describes state of virtual processors - VCPUs; easier for hypervisor to know if particular operation should not cause trap into root mode, just handled by privilege layer in non-root
- extended page tables and tagged TLBs (translation lookaside buffer) with VM ids
- multiqueue devices and interrupt routing
- security and management support
- additional instructions to support above features