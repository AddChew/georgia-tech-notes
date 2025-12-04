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