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
