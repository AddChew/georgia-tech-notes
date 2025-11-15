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