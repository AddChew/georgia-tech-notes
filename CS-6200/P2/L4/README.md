# P2L4

## User vs Kernel Level Threads

## Interupts vs Signals

Interupts (from external)
- events generated externally by components other than CPU
- determined by physical platform
- asynchronous
- associated with CPU (delivered to whole CPU)
- interupt handler lookup table used to manage which handler should be run, stores mapping between the interupt handler ID and the starting address of the interupt handling routines

Signals (from within)
- events triggered by CPU and software running on it
- determined by OS
- can be synchronous or asynchronous
- associated with process (delivered to individual processes)
- SIGSEGV: happens when thread tries to access illegal memory
- signal handler lookup table used to manage which handler should be run, stores mapping between the signal handler ID and the starting address of the signal handling routines

Similarities
- Have a unique ID depending on hardware/OS
- Can be masked (ignored) and disabled/suspended via corresponding mask
    - per-CPU interupt mask, per-process signal mask
- If interupt/signal is enabled, trigger corresponding handler
    - interupt handler is on system level by OS
    - signal handler is on process level by process

## Masks

Interupt masks
- per CPU
- if mask disable interrupt, hardware interupt routing mechanism will not deliver interrupt to CPU

Signal masks
- per execution context (user level thread on top of kernel level thread)
- if mask disable signal, kernel sees mask and will not interrupt corresponding thread

## Interupt in Multi Core/CPU system

- interrupt can be directed to any CPU that has it enabled
- may set interrupt on just 1 single core, avoid overhead related to interrupt handling on other cores, improved performance

## Signal Types

One-shot signals
- N signals pending == 1 signal pending, handled at least once, possible that be it N or 1 signal happen, only handled once
- handling routine must be explicitly re-enabled, once triggered will only execute once, subsequent time will execute the default OS action, have to manually enable again

Real-time signals
- N signals raised, then handler is called N times

## Interrupts as Threads

To solve the deadlock issue, we can:
- Allow interrupts to become full fledged threads (i.e. run interrupt handler on a separate independent thread from the "interrupted" thread, "interrupted" thread can continue to execute)

Tradeoffs
- Dynamic thread creation is expensive
- Decision
    - If handler does not lock, execute on interrupted thread's stack
    - If handler can block, turn it into real thread
- Optimization
    - precreate and preinitialize threads for interrupt routines

## Threads and Signal Handling

Setup
- Mask on user level thread (only visible on user level)
- Mask on kernel level thread (only visible on kernel level)

Problem
- User level disable mask, need some way to synchronize the disable mask on kernel level as well (i.e. do not want to perform a system call each type user level signal mask is modified as it is expensive)

Solution
- Abstraction layer via thread library on user level, which will provide library handling routine (library has full visibility of the state of the masks on user level threads)