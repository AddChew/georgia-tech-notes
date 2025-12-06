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

### Cloud Computing

Traditional approach
- buy and configure resources: determine capacity based on expected peak demand
- cons:
    - when demand > capacity, there will be dropped requests + lost opportunity

Ideal Cloud
- capacity scales elastically with demand
- scaling is instantaneous, both up and down
- cost is proportional to demand, to revenue opportunity
- all of the above should happen automatically without manual intervention
- resources can be accessed anytime from anywhere
- cons: don't "own" the resources

#### Cloud Computing Requirements

- on-demand, elastic resources and services
- fine grained pricing based on usage
- professionally managed and hosted
- API based access (ubiquitous)

#### Cloud Computing Overview

- shared resources: infrastructure and software/services
- APIs for access and configuration: web-based, libraries, command line (ubiquitous)
- billing and accounting services
- managed by cloud provider

#### Why does cloud computing work?

- Law of large numbers
    - per customer there is large variation in resource needs
    - average across many customers is roughly constant
- Economies of scale
    - unit cost of providing resources/services drops at bulk

#### Cloud Deployment Models

Public cloud
- third party customers/tenants
- anyone with credit card can use cloud service/infrastructure provided by cloud provider
- customer/tenant does not own the infrastructure, rent from cloud provider, cloud provider != customer/tenant
- i.e. AWS

Private cloud
- leverage technology internally
- everything owned by same entity (tenant = cloud provider)
- cloud technology used to enable provisioning

Hybrid cloud (private + public)
- failover, dealing with spikes, testing

Community cloud
- special type of public cloud used by certain type of customers

#### Cloud Service Models

On-Premise
- manage everything yourself

Infrastructure as a Service (IaaS)
- manage the app, data, runtime, middleware, OS yourself
- i.e. Amazon EC2

Platform as a Service (PaaS)
- only manage the app and data yourself
- i.e. Google App Engine

Software as a Service (SaaS)
- cloud manage everything for you
- i.e. Gmail

#### Requirements for Cloud

- "Fungible" resources: resources can easily be repurposed to support different customers
- Elastic, dynamic resource allocation methods
- Scale: management at scale, scalable resource allocations
- Dealing with failures
- Multi-tenancy: performance and isolation
- Security

#### Cloud Enabling Technologies

- Virtualization
- Resource provisioning (scheduling): mesos, yarn
- Big data 
    - processing: Hadoop MapReduce, Spark
    - storage:
        - distributed file system
        - NoSQL, distributed in-memory cache
- Software defined isolation/partitioning: networking, storage, data centers
- Monitoring: real time log processing (Flume, CloudWatch, Log insight)

#### Cloud as a Big Data Engine

- data storage layer
- data processing layer
- caching layer
- language front-ends (i.e. SQL for querying)
- analytics libraries (i.e. machine learning)
- continuously streaming data