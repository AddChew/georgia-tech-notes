# P4L1: Remote Procedural Calls (RPC)

Common steps related to remote IPC gave rise to RPC - intended to simplify the development of cross-address space and cross machine interactions

## Benefits
- RPC provides a higher level interface for data movement and communication
- Error handling
- Hides complexities of cross machine interactions

## Requirements
- Client/server interactions
- Procedural call interface - RPC: sync call semantics (blocking call)
- Type checking
    - error handling
    - packet bytes interpretation
- Cross machine conversion: RPC must ensure data is correctly transported, must perform the necessary conversions among the 2 machines
- Higher level protocol
    - support different transport protocols (i.e. TCP, UDP)
    - support access control, fault tolerance ...

## High Level Flow
1. Client call to procedure
2. Client stub builds message
3. Message is sent across the network to server
4. Server OS handles message in server stub
5. Server stub unpacks the message
6. Server stub make local call to procedure
7. Server stub builds results message and send it to client across network

## RPC Steps

-1: register: server "registers" procedure, args types, location, ..
0. bind: client finds and "binds" to desired server
1. call: client makes RPC call; control passed to stub, client code blocks
2. marshal: client stub "marshals" arguments (serialize args into buffer)
3. send: client sends message to server
4. receive: server receives message, passes message to server stub; perform checks like access control
5. unmarshal: server stub "unmarshal" args (extract args and creates data structs)
6. actual call: server stub calls local procedure implementation
7. result: server performs operation and computes result of RPC operation
8. result passed to server stub, follow similar reverse flow to send back to client

## Interface Definition Language (IDL)

Serves as a protocol of how the agreement between client and server will be expressed

Describe the interface the server exports:
- procedure name, arg and result types
- version no

RPC can use IDL that is
- language agnostic: XDR in SunRPC
- language specific: Java in Java RMI

IDL is just the interface, not used for actual implementation of the service.

## Marshalling and Unmarshalling

Client: Marshalling process needs to encode data into some agreed format so that it can be correctly interpreted on receiving side. Encoding specifies data layout when its serialized to the byte stream.

Server: Unmarshalling - decode data, opposite of marshalling

## Binding and Registry

Registry = database of available services
- distributed: any RPC service can register
- machine-specific:
    - for services running on same machine
    - client must know machine address: registry provides port number needed for connection

## Pointers

Approaches
1. No pointers
2. Serialize pointers: copy referenced ("ponted to") data structure to send buffer

## Handling Partial Failures

Special general RPC error notification (signal, exception, ...) to catch all possible ways a RPC call can (partially) fail (catch all error). Unable to provide exact failure reason.

## SunRPC

Design choices
- binding: per machine registry daemon
- IDL: XDR for interface specs and encoding - language agnostic
- pointers: allowed and serialized
- failures: automatic retries + return as much info as possible

Overview
- client server via procedure calls
- interface via XDR (.x file)
- rpcgen compiler: converts .x to language specific stubs
- server registers with local registry daemon
- registry (per machine)
    - name of service, version, protocols, port number
- binding creates handle
    - client uses handle in calls
    - RPC runtime uses handle to track
    - per client RPC state
- client and server can be on same or different machines

Registry
- RPC daemon = portmapper
- launch command: `/sbin/portmap` (need privileges)
- query command to check registered services: `rpcinfo -p`
    - /usr/sbin/rpcinfo -p
    - get program id, version, protocol (tcp/udp), socket port number, service name, ...
    - portmapper runs with tcp and udp on port 111

Binding - Client
```
// in client
CLIENT* clnt_create(char *host, unsigned long prog, unsigned long vers, char* proto);
```

```
// square example
CLIENT* clnt_create(rpc_host_name, SQUARE_PROG, SQUARE_VERS, "tcp");
```

CLIENT type
- client handle
- stores info like status, error, authentication

## XDR

Example
- client: send x
- server: return x**2
- XDR (.x file) describes
    - datatypes
    - procedures (name, version ...)

```
struct square_in {
    int arg1;
};
struct square_out {
    int res1;
};

program SQUARE_PROG { /* RPC service name */
    version SQUARE_VERS {
        square_out SQUARE_PROC(square_in) = 1; /* proc 1 */
    } = 1 /* version */
} = 0x31230000; /* service id */
```

Compiling XDR
- use rpcgen compiler
```bash
rpcgen -c square.x # command to generate c code based on .x file, not thread safe!!!
rpcgen -c -M square.x # command to generate multithread safe code, does not create a multithreaded server, have to do manually on linux, if on solarix, add -a flag, will create MT server for you as well
```
- outputs
    - square.h: data types and function definitions
    - square_svc.c: server stub and skeletion
        - main: registration/housekeeping
        - square_prog_1:
            - internal code, request parsing, arg marshalling
            - version
        - square_proc_1_svc: actual procedure; must be implemented by developer
    - square.clnt.c: client stub
        - square_proc_1: wrapper for RPC call to square_proc_1_svc
    - square_xdr.c: common marshalling routines

Summary
- from .x -> generate header, stubs, ..
- developer
    - server code: implementation of square_proc_1_svc
    - client side: call square_proc_1()
    - include .h
    - link with stub objects
- rpc runtime - does the rest
    - OS interactions, communication management

Supported Data Types
- default types: char, byte, int, float, ...
- additional XDR types: 
    - const (#define in C)
    - hyper (64 bit int)
    - quadruple (128 bit float)
    - opaque (C byte): uninterpreted binary data
- fixed length array: i.e. int data[80]
- variable length array: 
    - i.e. int data<80>, angular brackets denote the max length, translates into data structure with "len" and "val" fields; "val" field denotes the pointer address where the data structure is stored
    - except for strings
        - string line<80>: C pointer to char
        - stored in memory like normal null terminated string
        - encoded (for transmission) as a pair of length and data

Routines
- marshalling/unmarshalling
    - found in square_xdr.c
- clean up
    - xdr_free()
    - user defined freeresult procedure
    - i.e. square_prog_1_freeresult
    - called after results are returned automatically by rpc runtime

## Encoding

Following components are sent in message packet
- Transport header: i.e. TCP, UDP
- RPC header
    - service procedure ID, version number, request ID
- Actual data
    - arguments or results
    - encoded into a byte stream based on data type