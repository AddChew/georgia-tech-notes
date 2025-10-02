# P2L5

## Usefulness of threads

 - Depends on metrics
 - Depends on workload

## Event Driven Model

- Single process, single thread
- How it works is that the event dispatcher will offload tasks to the corresponding handlers
- Use connection switching to achieve concurrency, for blocking handler, return control back to event dispatcher for it to perform other actions
- Process request until wait necessary then switch to another request
- For multi CPU environment, we setup multiple event driven processes

Benefits
- single address space, single flow of control
- smaller memory requirement, no context switching between threads/processes
- no synchronization

Problem
- Blocking IO calls in handlers could block the entire event driven process

Solution
- Asynchronous IO operations (compatible with select, poll, epoll)

What if async calls are not available?
- Use helpers - designated for blocking IO operations only, offload blocking operations to the helper (can be process or thread based helpers)
- Communication between helper and event dispatcher via pipe/socket (i.e. file descriptor based so select, poll still works)
- helper blocks but main event loop (and process) not blocked