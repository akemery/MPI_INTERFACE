# MPI_INTERFACE
This software provides an MPI interface allowing processes called nodes to communicate with each other and share services provides by each node. Each node has
 
An identifier that allows it to be distinguished from other nodes.
A buffer in which messages for the node are dropped

The primary node has a main node in which all nodes drop messages destined for other nodes. The primary node then copies the message to the destination node buffer.

To be allowed to send messages, a new node must be registered. During the registering, the node transmits to the main node the list of its services. It receives in return an identifier and the necessary information to enable it to send and receive a message from/to the other nodes.

The associated node can thus access the list of available nodes and have access to the services provided by each of these nodes.

The structure of a message is:
/* Definition of the message format*/

struct message{
  long source;          /* senders identity */
  long dest;            /* receiver's identity */
  long opcode;          /* requested operation */
  long count;           /* number of byte to transfer */
  long offset;          /* position in file to start I/O */
  long result;          /* result of the operation*/
  char name[MAX_PATH];  /* name of file being operated on*/
  char data[BUF_SIZE];  /* data to read or written*/
};
