### Variables:

- server_fd is the socket descriptor for the server; it represents a communication endpoint used by the server to listen for incoming connections.
- new_socket is the socket descriptor created when the server successfully accepts a client connection. This socket is used to send and recieve data with the connected client.

### Functions:

1. socket(int domain, int type, int protocol)

   - used to create a new socket, intializes a communication endpoint for a network connection
   - the domain represents the address family (IPv4, IPv6)
   - the type represents the socket type (TCP, UDP)
   - the protocol represents the protocol to use, 0 represents the default for the given type and domain

2. setsockopt(int socket, int level, int option_name, const void \*option_values, socklen_t option_len)

   - allows configuration of options for a socket
   - the level represents the protocol level at which the option resides
   - the option_name represents the specific option to set
   - the option_value represents a pointer to the value of the option
   - the option_len represents the length of the option value

3. bind(int sockfd, const struct sockaddr \*addr, socklet_t addrlen)

   - assigns a specific address to a socket, which is needed for a server to listen on a given IP and port
   - the sockfd represents a socket descriptor (returned by the socket method)
   - the addr represtns a pointer to a sockaddr structure that contains the address information
   - the addrlen represents the size of the address structure

4. listen(int sockfd, int backlog)

   - marks a socket as ready to accept incoming connection requests and puts the socket in a listening state for a specific backlog or queue size for incoming connections
   - the sockfd represents the socket descriptor
   - the backlog represetns the maximum length of the queue of pending connections
     - new connections requested while the queue is full will be refused until space becomes available

5. accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)

   - accepts an incoming connection request on a socket that has been set to listeining mode, creates a new socket for the accepted connection which can then be used for data exchange with the client
   - the sockfd represents the socket descriptor of the listening socket
   - the addr represents a pointer to a sockaddr structure which will hold the address of the clinet making the connection
   - the addrlen represents the size of teh sockaddr structure and continas the actual length of the clients address on sucess

6. htons()

   - converts an unsigned short integer from host byte order to network byte order
   - network byte order is always big-endian but host byte order varies based on the machine
   - stands for Host To Network Short

### An example of the sockaddr_in struct:

```c
struct sockaddr_in {
    sa_family_t    sin_family;   // Address family (AF_INET for IPv4)
    in_port_t      sin_port;     // Port number (in network byte order)
    struct in_addr sin_addr;     // IP address (in network byte order)
    char           sin_zero[8];  // Padding to make the struct the same size as sockaddr
};
```

- If you wanted to use IPv6, than you would use AF_INET6 instead of AF_INET
- Note: network byte order is big-endian

### Socket Options Used

1. SOL_SOCKET

- a constant that represents the level at which a socket option is set, indicates that the option being set with the setsockopt() function applies to the socket itself

2. SO_REUSEPORT

- a socket option that allows multiple sockets on the same host to bind to the same port
  - useful when you want multiple to allow multiple processes or threads to listen on the same port for incoming traffic (think load balancing)

3. SOCK_STREAM

- a constant that indicates the type of socket you want to create, tells the system to create a strem-oriented socket, which is used for two-way communicatino
  - SOCK_STREAM coresponds to TCP sockets
  - could be replaced with SOCK_DGRAM if you wanted to use UDP
