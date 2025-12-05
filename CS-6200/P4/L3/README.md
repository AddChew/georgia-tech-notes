# P4L3: Distributed Shared Memory

## "Peer" Distributed Applications

All nodes are "peers", have some designated nodes to perform the management tasks. Different from peer-to-peer system where even the control and management tasks is done by all nodes.

Each node:
- owns state
- provides services

## Distributed Shared Memory (DSM)

Service that manages memory across multiple nodes, application running on top has illusion that is is running on a shared memory machine.

Each node:
- owns state: portion of physical memory
- provides service:
    - memory read/writes from any node
    - consistency protocols

Importance
- Permits scaling beyond single machine memory limits
    - Pros: more "shared" memory at lower cost
    - Cons: slower overall memory access
    - Possible due to support from commodity interconnect technologies (RDMA - remote direct memory access) which provides low latency for remote memory access

## Hardware vs Software DSM

Hardware supported
- relies on interconnect
- OS manages larger physical memory
- NICs (Network Interface Card) translate remote memory accesses to messages
- NICs involved in all aspects of memory management
- Limitations: such hardware is very expensive

Software supported
- everything done by software:
    - software have to detect if memory access is local/remote, create and send messages to appropriate node, accept messages from other nodes and perform appropriate memory operation for them
    - provide memory sharing and consistency support
- done on OS level or programming language runtime

## DSM Design

### Sharing Granularity

- cache line granularity: overhead too high for DSM
- variable granularity: still to high overhead
- page granularity (OS level):
    - OS track when pages are modified and trigger necessary messages to be exchanged with remote nodes on page modification
    - page larger (i.e. 4kb), amortize cost of remote access
- object granularity (language runtime):
    - only applicable to programming languages with DSM support

Pitfalls
- False sharing:
    - Two nodes, each with variable X and Y stored on the same page
    - Each node only cares about one of the variables and not the other
    - But since X and Y stored on the same page, OS sees their modifications as concurrent writes to same page and will trigger the coherrence mechanisms (i.e. cache invalidation)
    - Generate unnecessary coherrence overheads for maintaining consistency
    - Programmer responsiblity to control how data is allocated and laid out on pages

### Access Algorithm

- single reader/writer (SRSW)
- multiple readers/single writer (MRSW)
- multiple readers/multiple writers (MRMW)
    - ensure reads return the correctly written; i.e. most recent value
    - writes performed are correctly ordered
    - to present consistent view of distributed state

### Migration vs Replication

DSM performance metric = access latency

Achieve low latency through:
- Migration
    - makes sense for SRSW
    - requires data movement (copy data over to remote node, incur overhead)
- Replication (caching)
    - more general
    - requires consistency management
    - for many "concurrent" writes, overheads may be too high

### Consistency Management

DSM similar to shared memory in symmetric multiprocessing (SMP)

In SMP
- write-invalidate: each time write to cache in one process, invalidate cache in other processes
- write-update: each time write to cache in one process, update cache in other processes with new value
- coherence operations triggered on each write, too high overhead

Possible mechanisms
- Push invalidations when data is written to
    - proactive
    - eager
    - pessimistic
- Pull modification info periodically
    - on demand (reactive)
    - lazy
    - optimistic

When the above mechanisms get triggered depends on the consistency model for the shared state

### Architecture

Page-based, OS supported
- distributed nodes, each with own local memory contribution
- pool of pages from all nodes form the global shared memory
- every address in memory pool uniquely identified based on node identifier + page frame number of that particular memory location (node id + page id)
- "home" node = node where page is located at (local)

If MRMW
- need local cache for performance (latency)
- home (or manager) node drive coherence operations
- all nodes responsible for part of distributed memory (state) management
- "home" node
    - keeps state: pages accessed, modified, caching enabled/disabled, locked ...
    - mechanism to separate notion of home node from so-called owner
        - owner is the node that currently own the pages, i.e. exclusive writer, node that can control all of the state updates and drive consistency related operations
        - owner might be different from home node, owner might change with whoever is accessing the page currently (varies from time to time and node to node)
        - role of home node is to track who is current owner of page

Replication
- for load balancing, performance, reliability
- triplicate shared state on original machine, nearby machine and another remote machine
- consistency of replicas controlled via home node or some designated management node

### Indexing Distributed State

To identify where a page is, have to maintain some metadata. 

Each page (object) has
- address (node id + page frame number)
- node id = "home" node

Global map (replicated)
- page (object) id, index into mapping table, map to home/manager node id
- manager map available on each node
- benefits of this approach is that if we want to update the managers of the pages, just have to update the mapping table, no need for changes to the page (object) identifiers

Metadata for local pages (partitioned)
- per page metadata distributed across managers

### Implementation

Problem: DSM must "intercept" accesses to DSM state
- to send remote messages requesting access
- to trigger coherence messages
- overheads should be avoided for local, non-shared state (pages)
- dynamically "engage" and "disengage" DSM when necessary, i.e. "engage" for remote memory access and "disengage" for local memory access

Solution: use hardware memory management unit (MMU) support
- trap in OS if mapping invalid or access is not permitted
- perform access to remote memory, no valid mapping from local virtual address to remote physical address: trap generated and will pass page information to DSM layer to send message
- cached content: DSM ensure write protected, if anyone try to modify it, will generate a trap and pass to DSM to perform necessary coherence operations
- maintain other useful MMU information (i.e. dirty pages)

### Consistency Model

