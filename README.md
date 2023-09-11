# Project-3-Socket-File
Side project #3! I wanted to go further with my socket project  #2, and instead of just send messages. In this project, I incorparted creating a socket in C that can send files! 

Here is a more detailed explanation of how my code works: 
Host (Server) Side:

The server code begins by including necessary header files such as <stdio.h>, <stdlib.h>, <string.h>, <unistd.h>, <sys/types.h>, <sys/socket.h>, and <netinet/in.h>.

It defines a constant PORT with the port number (8080) on which the server will listen for incoming connections.

The code initializes various variables, including serverFileDescriptor (for the server socket), newSocket (for the new socket created when a client connects), and valueRead (for tracking bytes read).

It sets up a struct sockaddr_in called address to store server address information.

A buffer called buffer is created to hold chunks of data read from or written to a file. The buffer size is defined as BUFFER_SIZE (1024 bytes).

It declares a pointer to represent a file (FILE *file) that the server will write data to, based on the file sent by the client.

The code creates a socket for the server using the socket() function, specifying the address family (AF_INET for IPv4), socket type (SOCK_STREAM for TCP), and protocol (0 for the default).

It sets socket options to allow reuse of the address and port, which can help improve performance and reliability if the socket is not properly closed.

Server address information is initialized, including the address family, IP address (INADDR_ANY to bind to any available network interface), and port number. The address is then bound to the server socket using the bind() function.

The server listens for incoming connections using the listen() function, allowing up to three pending connections in the socket's queue.

When a client connects, the accept() function is called to accept the incoming connection, creating a new socket (newSocket) for communication with that client. If the connection fails, an error message is printed.

The server receives the file name from the client using the read() function and stores it in the buffer.

It opens the file for writing in binary mode ("wb"), with the file name obtained from the client. If the file cannot be opened, an error message is printed.

The server enters a loop where it continues to read data from the client socket using read() and writes this data to the file using fwrite() until the end of the file is reached or an error occurs.

After successfully receiving the file, the server prints "File Transfer Successful" and proceeds to close the file and both sockets (newSocket and serverFileDescriptor).

Client Side:

The client code includes necessary header files and defines constants for the target server's IP address (TARGET_IP_ADDR) and port number (TARGET_PORT).

It creates a socket for the client using the socket() function, specifying the same address family (AF_INET) and socket type (SOCK_STREAM).

The code prompts the user to enter the file path of the file they want to send.

It sets up a struct sockaddr_in called serverInfo to store the server's address information.

The client attempts to connect to the server using the connect() function. If the connection fails, an error message is printed, and the program exits.

It opens the specified file for reading ("r" mode).

The client sends the file name to the server using the send() function.

It reads the file data in chunks, sends each chunk to the server using send(), and continues until the entire file is sent.

After successfully sending the file, the client closes the socket.

In summary, the host (server) side of the code listens for incoming connections, accepts a file from the client, and saves it to the server's filesystem. The client side connects to the server, reads a file from its own filesystem, and sends it to the server. This code facilitates file transfer from a client to a server over a TCP/IP socket connection.
