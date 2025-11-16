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

### Compromise

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

