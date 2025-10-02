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