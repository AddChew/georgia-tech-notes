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

## Files vs Directories

2 types of files:
- regular files vs directories
- choose different policies for each
- i.e. session semantics for files, unix for directories
- i.e. less frequent write back for files than directories

## Replication vs Partitioning

replication
- each machine holds all files
- pros:
    - load balancing, availability, fault tolerance
- cons:
    - writes become more complex
        - synchronously to all replicas
        - or write to one, then propagated to others
    - replicas must be reconciled if there are any differences among state of file on different replicas, i.e. voting

partitioning
- each machine has subset of files
- pros:
    - greater availability vs single server DFS (each server hold less files, able to respond to request faster due to lower workload)
    - scalability with file system size (just add more machines to scale)
    - single file writes simpler
- cons:
    - on failure, lose portion of data
    - load balancing harder, if not balanced then hot spots possible

## Network File System

- use rpc to transmit the file contents; on open, server returns file handle
- 'stale' file handle - happens when
    - file/directory removed/renamed
    - server crash

Versions
- NFS v3 = stateless but typically used with extra modules to support caching and logging
- NFS v4 = stateful

Caching
- session based for non-concurrent usage
- periodic upates
    - default: 3s for files, 30s for directories
- NFS v4: delegation to client for period of time (avoid update checks)

Locking
- lease based mechanism: server assigns client a time period which lock is valid; client responsibility to make sure it either releases the lock within leased amount of time or explicitly extend lock duration
- NFS v4: supports reader writer lock (share reservation)

## Sprite Distributed File System

### Access pattern analysis
- 33% of file accesses are writes: caching ok, but write through not sufficient
- 75% of files open less than 0.5s, 90% of files open less than 10s: session semantics too high overhead
- 20 - 30% of new data deleted within 30s, 50% of new data deleted within 10min: write back on close not really necesary
- file sharing is rare: no need to optimize for concurrent access but must still support it

### Design
- cache with write back
    - every 30s write back blocks that have not been modified for last 30s
    - when another client opens file, get dirty blocks
- every open goes to server, directories cannot be cached on client
- on "concurrent write", disable caching for file

### Sprite sharing semantics
- sequential write sharing = caching and sequential semantics
- concurrent write sharing = no caching

### File access operations in Sprite
- all open go through server
- all clients cache blocks
- writer keeps timestamps for each modified block
- i.e. w2 sequential writer (sequential sharing)
    - server contacts last writer for dirty blocks
    - if w1 has closed file, update version
    - w2 can now cache file
- i.e. w3 concurrent writer (concurrent sharing)
    - server contacts last writer for dirty blocks
    - since w2 has not closed, disable caching

Server, per file has to store the following info
- readers
- writer
- version
- cache?

Client, per file has to store the following info
- cache?
- cached blocks
- timer for each dirty block
- version