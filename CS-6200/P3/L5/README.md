# P3L5: I/O Management

## Basic I/O Features

Interface: Command registers
- command
- data transfers
- status

Internals: 
- microcontroller = device's CPU
- on device memory - DRAM/SRAM
- other processing logic - special chips/hardware needed on device

Devices connect to CPU via CPU device interconnect through PCI (Peripheral Component Interconnect). PCI express > PCI-X > PCI in terms of performance, more bandwidth, faster, lower access latency, supports more devices

Other types of interconnects
- SCSI bus
- peripheral bus
- bridges that handle differences between different types of interconnects

## Device Drivers

- Specific to device type, per each device type
- responsible for device access, management and control
- provided by device manufactuers per OS version
- each OS standardize their interfaces to a device driver's via device driver framework, so device manufacturers develop their device driver within that framework
    - device independence
    - device diversity

## Device Types

Block: disk
- read/write blocks of data
- direct access to arbitrary block

Character: keyboard
- get/put character

Network devices
- stream data of flexible sizes

OS representation of a device = special device file. Benefits of this is that OS can use file manipulation mechanisms to access these devices (handled in device specific manner).

In unix systems, devices appear under the directory /dev in tmpfs and devfs.

Following commands all print something to lp0 printer device
- cp file > /dev/lp0
- cat file > /dev/lp0
- echo "Hello world" > /dev/lp0

Linux supports a number of pseudo (virtual) devices that provide special functionality to a system.

Command to accept and discard all output: /dev/null
Command to produce variable length string of pseudo random numbers: /dev/random

## CPU Device Interactions

Device registers appear to CPU as memory locations at specific physical address.

Memory-mapped I/O
- part of "host" physical memory dedicated for device interactions
- memory for I/O controlled by base address registers (BAR)

I/O port
- dedicated in/out instructions for device access
- target device (I/O port) and value in register

Communication from Device to CPU:

Interupt
- Pros: can be generated as soon as possible
- Cons: overhead due to interupt handling steps

Polling
- Pros: OS can choose when it is convenient to poll
- Cons: delay or CPU overhead

## Programmed I/O

- method to request an operation from a device
- requires no additional hardware support
- CPU "programs" the device via
    - command registers
    - data movement

Example: Network Interface Card
- write command to request packet transmission
- copy packet to data register
- repeat until packet sent in loop

## Direct Memory Access (DMA)

- relies on DMA controller hardware support
- CPU "programs" the device
    - command via command registers
    - data movement via DMA controls

Flow
- write command to command register to request packet transmission from CPU to device
- configure DMA controller with in-memory address and size of packet buffer
- data buffer must be in physical memory until transfer completes (memory regions are pinned, non-swappable)
- Pros: less steps, involves 1 store instruction (command) and 1 DMA configure
- Cons: 
    - DMA configuration is more complex, more cycles to configure

For smaller transfers, programmed I/O better than DMA

## Typical Device Access

Flow
- user process -> kernel: system call (i.e. send data, read file)
- kernel -> driver: in-kernel stack (i.e. form packet, determine disk block)
- driver -> device: driver invocation to perform configuration of request to device via programmed I/O or DMA (i.e. write request record, issue disk head movement/read)
- device: perform request (i.e. perform transmission, read block from disk)
- device -> driver -> kernel -> user process: response chain

## OS Bypass

In some cases:
- device registers/data directly accessible to user level
- OS configures then out of the way
- "user-level driver" library (provided by device manufacturer)
- OS retains coarse-grain control (i.e. enable/disable device, permission to add more processes to use device)
- OS relies on device features, device has to have sufficient registers for OS to map some registers to user processes and some registers for OS to perform coarse-grain control operations
- device has to have demultiplex capability - when multiple user processes access the device at same time, need to have a mechanism to send and receive to the correct processes (this mechanism typically handled by OS, but if we bypass OS, then device has to be able to handle it)

## Sync vs Async Access

Sync I/O: process blocks

Async I/O:
- process continues
- later
    - process checks and retrieves results
    OR
    - process is notified (by OS/device) that operation completed and results are ready - process doesnt need to poll

## Block Device Stack
