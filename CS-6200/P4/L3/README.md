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