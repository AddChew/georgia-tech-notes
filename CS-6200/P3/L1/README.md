# P3L1: 

## Scheduling Strategies

First come first serve (FCFS/FIFO)
- Benefit: simple scheduling
- Assign tasks immediately as they come in
- Low overhead in OS scheduler

Shortest Job First (SJS)
- Assign simple tasks first
- Benefit: maximize throughput

Complex tasks first
- Assign complex tasks first
- Benefit: maximize resource utilization (i.e. CPU, devices, memory)

## CPU Scheduler

- Decides how and when processes/threads access shared CPUs
- Schedules tasks running user-level processes/threads and kernel-level threads
- Choose a task from ready queue and schedule it onto the CPU
- Run when:
    - CPU is idle
    - new task created/becomes ready, goal is to check whether any of these tasks are of higher priority and should interupt the task that is currently executed on the CPU
    - timeslice expire (i.e. each task assigned a predetermined amount of time on the CPU, when time run out, need to run scheduler to choose next task to run on CPU)
- Perform context switch when the task on the CPU has to be changed
- Scheduling policy/algorithm decides how task is selected
- How scheduler implements scheduling policy depends on runqueue data structure

## Scheduling Policies

### Run to completion scheduling

- Known execution times
- No interuption (preemption), let the task run till completion
- Single CPU

Metrics:
- Throughput
- Average job completion time
- Average job wait time
- CPU utilization

First-come-first serve (FCFS)
- schedule tasks in order of arrival
- runqueue == queue data structure (FIFO)
- simple but wait time for tasks is poor even if there is just 1 long task in queue that arrived ahead of shorter task

Shortest Job First (SJF)
- schedule task in order of their execution time
- runqueue == ordered queue or tree

### Premptive scheduling

- Interupt to run short task first, then resume long task if combined with SJF policy
- Use heuristics based on history to predict a task running time in the future

Priority scheduling
- tasks have different priority levels
- run highest priority next (preemption)
- Data structure
    - multiple runqueues, different runqueue for each priority level, scheduler select task from queue of highest priority
    - tree ordered based on priority
- Issues
    - low priority task stuck in runqueue (starvation)
    - solution is "priority aging" where priority = f(actual priority, time spent in queue)
    - eventually task will run, prevent starvation

Priority Inversion
- priorties inverted
- happens due to lower priority task having the mutex lock, but suspended due to higher priority task, but higher priority task cannot proceed as it also requires the mutex lock. As a result, lower priority task complete in front of higher priority task
- solution:
    - temporarily boost priority of mutex owner
    - lower priority once mutex released


### Round Robin Scheduling:

For jobs with same priority
- tasks may yield to wait on IO (unlike FCFS)

For jobs with different priorities
- include preemption of lower priority task to run higher priority task

Interleaving
- timeslicing
- each task allocated x time unit (timeslice) to execute, after which will preempted to run the next task
- timeslice/time quantum = max amount of uninterupted time given to a task
- task may run less than timeslice time: 
    - wait on IO, synchronization, placed on queue
    - higher priority task come in
- use of timeslice allows us to interleave tasks, timeshare the CPU
- Benefits:
    - shorter tasks finish sooner
    - more responsive (shorter average wait time)
    - lengthy IO operations can be initiated sooner
- Trade offs:
    - overheads due to interrupt, schedule, context switch
    - as long as timeslice >> context switch time, minimize overhead

### Time Slicing

CPU bound
- prefer longer timeslice
- longer timeslice, less context switch overhead
- higher CPU utilization and throughput
- better throughput and completion time at the cost of longer wait time
- but for cpu bound task, wait time not as important metric

IO bound
- prefer shorter timeslice (earlier and more interweaving)
- IO bound tasks can issue IO operations earlier
- keep CPU and device utilization high
- better user perceived performance

## Runqueue Data Structure

CPU and IO bound task to have different timeslice values
- Same runqueue, check type
- 2 different data structures 

Multiqueue data structure
- most IO intensive task, highest priority
- medium IO intensive (mix of IO and CPU), medium priority
- CPU intensive, lowest priority
- benefits:
    - timeslicing benefit for IO bound tasks
    - timeslicing overhead avoided for CPU bound tasks
- how to tell if task is IO/CPU bound? (Multi-Level Feedback Queue (MLFQ))
    - push task through different timeslice
    - start with highest priority, shortest timeslice, if task yields voluntarily before timeslice end, correct choice, keep task at this level
    - if task use up whole timeslice, push task to lower level (medium priority)
    - if task still use up whole timeslice of medium priority, push task to lowest level (CPU intensive queue)
    - task in lower priority queue can get bumped up (priority boost) when releasing CPU due to IO work
    - different treatment of threads at each level
    - feedback mechanism to adjust task level

Linux O(1) Scheduler

- constant time to select/add task in queue regardless of task count
- preemptive, priority based
- 140 levels, 0 highest priority
- real time task (0 - 99)
- timesharing (100 - 139)
- user processes
    - default 120
    - can be adjusted by nice value (-20 to 19) via system call
- timeslice value
    - depends on priority
    - smallest for low priority
    - highest for high priority
- feedback
    - based on sleep time (waiting/idling)
    - longer sleep, interactive, priority - 5 (boost)
    - smaller sleep, compute intensive, priority + 5 (lowered)
- runqueue
    - 2 arrays of tasks, active array and expired array
    - active array
        - used to pick next task to run
        - constant time to add/select task
        - tasks remain in queue in active array until timeslice expires
            - if task yield CPU to wait on event or are prempted due to higher priority task, time spent on CPU substracted from total amount of time, if less than timeslice, still placed back on active queue
            - only when task consumes entire timeslice, then remove from active and move to expired queue
     expired array
        - inactive tasks, scheduler will not select tasks from here as long as there are still tasks in active array
        - when no more tasks in active array, swap active with expired
- limitations:
    - once task is on expired list, wont get scheduled untill all tasks in active list have executed for their timeslice, hurts performance of interactive tasks
    - no fairness guarantees

Completely Fair Scheduler (CFS) (default scheduler)

- 1 runqueue structure for all priority levels
- runqueue
    - red-black tree data structure
    - will self balance itself as nodes added/removed so that all paths of tree are approximately same size
    - tasks are ordered by virtual runtime (vruntime) = time spent on CPU
    - left node less time on CPU, right node more time
- scheduling algorithm
    - always schedule the task that spend the least time on CPU (i.e. leftmost node)
    - periodically update vruntime of task currently running on CPU
    - compare to leftmost vruntime
        - if smaller, continue running
        - if larger, preempt and place appropriately in tree
    - vruntime progress rate depends on priority and niceness
        - rate faster for lower priority
        - rate slower for higher priority
- performance
    - select task O(1)
    - add task O(log n)

## Scheduling on Multiple CPU systems

- CPUs have their own private cache (L1/L2)
- Shared last level cache (LLC)
- Shared system memory (shared memory multiprocessor)
- goal: try to schedule thread back on same CPU where it executed before to utilize the hot cache (keep tasks on same CPU as much as possible), cache affinity
- hiearchical scheduler architecture
    - load balancer to decide task goes to which cpu
    - per cpu scheduler with per cpu runqueue, repeatedly schedule tasks on given cpu as much as possible
- multiple memory nodes (non-uniform memory access - NUMA platform)
    - goal: use closer memory node to a "socket" of multiple processors (NUMA aware scheduling)
    - access to local memory node faster than access to remote memory node

## Hyperthreading (simultaneous multithreading - SMT)

- old design: 1 cpu, 1 register, have to save and restore state from memory
- have multiple registers, nothing has to be saved or restored
- still 1 cpu but very fast context switch

Fedorova paper
- co-schedule memory bound threads leads to wasted cpu cycles
- solution:
    - mix cpu and memory intensive threads
    - avoid/limit contention of processor pipeline
    - all components (cpu and memory) well utilized
    - leads to interference and degradation for compute bound threads but minimal

## CPU or Memory bound tasks?

- use historical information
- sleep time will not work
    - thread is not sleeping when waiting on memory
    - software takes too much time to compute
- from hardware counters (i.e. last level cache miss), estimate what kind of resources a thread needs
- memory bound, high cycles per instruction (CPI)
- cpu bound, 1 or low CPI

CPI experiment results (Only for a certain specific workload where tasks have spread out CPI, which is unrealistic)
- tasks with mixed different CPI, processor pipeline well utilized, high IPC
- tasks with uniform CPI, contention on some cores, wasted cycles on other cores, lower IPC
- conclusion: 
    - mixed CPI is good, high performance on system
    - CPI is a great metric

In practice, real workloads do not exhibit significant differences in their CPI values, so CPI will not be a useful metric.

Takeaways
- resource contention in SMTs for processor pipeline
- hardware counters can be used to characterize workload
- schedulers should be aware of resource contention not just load balancing
- LLC usage would be a better metric, pick a mix that does not cause contention on last level cache usage