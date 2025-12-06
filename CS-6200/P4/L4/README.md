# P4L4: Datacenter Technologies

## Internet Services

- Any type of service provided via web interface
- Components
    - presentation (web layout - your html and css): static content
    - business logic (user specific content): dynamic content
    - database tier: data store
- In multi-process configurations
    - some form of inter process communication (IPC) used, including RPC/RMI, shared memory

### Architectures

For scale ("scale out" architecture): multi-process, multi-node
- "Boss - worker": front-end (load balancer) distributes requests to nodes
- "all equal" - functionally homogeneous: all nodes execute any possible step in request processing for any request
- "specialized nodes "- functionally heterogeneous: nodes execute some specific step(s) in request processing for some request types

#### Homogeneous Architecture

- Each node can do any processing step
- Does not mean that each node has all data, just each node can get to all data
- Pros: keeps front-end (load balancer) simple, do not have to keep track which node can perform which particular action or can service which types of request
- Cons: Unable to benefit from caching

#### Heterogeneous Architecture

- Different nodes, different tasks/requests
- Data does not have to be uniformly accessible everywhere
- Pros: Benefit of locality and caching
- Cons: 
    - More complex front-end, have to decide how and where to route requests to
    - More complex management, when scaling out, have to decide on which task/request type nodes to add. Possible to have issue where some types of task/request nodes are the bottleneck, have long pending request queues while others idle