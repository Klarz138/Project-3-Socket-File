//Katherine Larsen
//CS 4420 Networks 
//The goal of this project is to create a socket to transport files (instead of messages)
//HOST SIDE
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 

#define PORT 8080

//buffer is used to hold a chunk of data read from or written to the file
#define BUFFER_SIZE 1024 

int main() 
{
  //Defining variables:
    //ServerFiledescriptor- Used to identify the socket when performing socket         operations (binding, listening, and accepting)
  
    //newSocket = Holds the file descriptor for the new socket that is created         when a client connects to the server. This will be used to communicate with        the client
  
    //valueRead = holds the number of bytes read by read function when receiving       data from the client. Will be used to check wheter the end of file has been        reached, or if an error is encounted while reading 
  
int serverFileDescriptor, newSocket, valueRead; 

  struct sockaddr_in address; 
  int addressLength = sizeof(address); 

  //Initlaize to 0 to ensure buffer is initially empty
  char buffer[BUFFER_SIZE] = {0};  

  //Pointer to represent a file that the server will write data to based on the file sent by the client
  FILE *file; 

  //Create socket file descriptor. Check ifserverfileDescriptor is false: 
//AF_INET = specfies the address domain of the socket 
//SOCK_SOCKET = specifies this a TCP socket. 
//0 = OS should chose the defaut prototocol for the add. family and socket type
if ((serverFileDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
{
  perror("1.) We're sorry but there was a failure in creating the socket. Please     try again.");
  exit (EXIT_FAILURE);
}

//Set socket options to allow resue of address and port this is important if the socket is not preoperlt closed. For example, if the prior instance of the server was terminated arbutly. By enabling the reuse of address and port, the server can resuse the address and the prot withouth having to wait for OS to release the resources --> improve perfomrance and reliablity of server. 

  //Creating variable to release address 
int opt = 1; 
if (setsockopt(serverFileDescriptor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, & opt, sizeof(opt))) 
{
  perror("2.) We're sorry but there was a setsockopt error.");
  exit(EXIT_FAILURE);
}

//Set socket options
address.sin_family = AF_INET; 
address.sin_addr.s_addr  = INADDR_ANY; 
address.sin_port = htons(PORT);

if (bind(serverFileDescriptor, (struct sockaddr *) & address, sizeof(address)) < 0)
{
  perror("Bind Failed");
  exit(EXIT_FAILURE);
} //end of if 

//Listen for incoming connections - I chose up a max number of 3 pending conections can be queued in the socket server. If more than 3 clients try to connect to the server simultanelously the additional connection attempts will be rejected. 
if (listen(serverFileDescriptor, 3) < 0)
{
  perror("3.) We're sorry but the listening has failed");
} //end of if 

//Accept the incoming connection 
if ((newSocket = accept (serverFileDescriptor, (struct sockaddr *) & address, (socklen_t*) & addressLength)) <0)
{
  perror("4.) We're sorry byt the accept has failed");
  exit (EXIT_FAILURE); 
}//end of if 

//Receive the file name from the client 
valueRead = read(newSocket, buffer, BUFFER_SIZE);
if (valueRead < 0)
{
  perror("5. We're sorry but the read has failed");
  exit(EXIT_FAILURE);
} // end of if 

  //Open the file for writing == wb is the mode in which the file will be opened. w indicitetated the file will be opened in write mode, while the b indivates the file will be creased as a binary file. 
file = fopen(buffer, "wb");
  if (file == NULL)
  {
    perror ("6.) We're sorry by the file has failed to open");
    exit(EXIT_FAILURE); 
  }//end of if 
  
while ((valueRead = read(newSocket, buffer, BUFFER_SIZE)) > 0) 
  {
  fwrite(buffer, 1, valueRead, file);
  }//end of while

  if (valueRead < 0)
  {
    perror("7.) We're sorry but the file cannot be read.");
    exit (EXIT_FAILURE); 
  } //end of if

  printf("\nFile Transfer Successful\n");
  //Close the file and socket 
  fclose(file); 
  close(newSocket);
  close(serverFileDescriptor);

}//end of main 
