# P4L2: Distributed File Systems (DFS)

Access files via Virtual File System (VFS) interface.

## DFS Models

1. client/server on different machines
2. file server distributed on multiple machines
    - replicated (each server has all files)
    - partitioned (each server has part of files)
    - both (files partitioned, each partition replicated)
3. files stored on and served from all machines (peers): blurred distinction between client and servers

## Remote File Service

### Extremes

1. Upload/Download Model
- Client downloads file locally
- Accesses/operations are done on client
- When client is done, upload modified file back to server
- pros: local read/writes at client can be done very fast
- cons: 
    - entire file download/upload even for small accesses
    - server gives up control

2. True Remote File Access
- every access to remote file, nothing done locally
- pros: file accesses centralized, easy to reason about consistency
- cons:
    - every file operation pays network cost
    - limits server scalability since every operation has to be done by server

### Compromise - Practical Model

1. allow clients to store parts of files locally (blocks)
    - pros: 
        - low latency on file operations
        - server load reduced, more scalable
2. force clients to interact with server (frequently)
    - pros: 
        - server has insights into what clients are doing
        - server has control into which accesses can be permitted, easier to maintain consistency
    - cons:
        - server more complex, requires different file sharing semantics (server has to perform additional tasks and maintain additional states for consistency guarantees)

## Stateless vs Stateful File Servers

- stateless = keep no state
    - ok with extreme models but cannot support 'practical' model
    - pros:
        - no resources are used on server side to maintain state
    - cons: 
        - cannot support caching and consistency management
        - every request self contained, more bits transferred
        - on failure, just restart

- stateful = keep client state
    - needed for 'practical' model to track what is cached/accessed
    - pros:
        - can support locking, caching, incremental operations
    - cons:
        - on failure, need checkpointing and recovery mechanisms
        - overheads to maintain state and consistency, depends on caching mechanism and consistency protocol

## Caching State in DFS

- locally, clients maintain portion of state (i.e. file blocks)
- locally, clients perform operations on cached state (i.e. open/read/write)
- requires coherence mechanisms
    - how: client/server driven - details depend on file sharing semantics
    - when: on demand, periodically, on open
- cache can be stored in
    - in client memory
    - on client storage device (HDD/SSD)
    - in buffer cache in memory on server

## File Sharing Semantics on DFS

- unix semantics: every write visible immediately
- session semantics (session = period between open and close):
    - write back on close(), update on open()
    - easy to reason, but may be insufficient for situations when clients want to concurrently share a file, write to it (have to open close repeatedly)
- periodic updates
    - client writes back periodically: clients have a "lease" on cached data (not necessarily exclusive like locking)
    - server invalidates periodically: provides bounds on "inconsistency"
    - augment with flush()/sync() API, provide option for client to explicitly flush updates to/sync state with server
- immutable files
    - never modify, always create new files
- transactions
    - all changes are atomic