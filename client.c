//Katherine Larsen
//Create and use a socket to send and receive files!

//CLIENT SIDE 


// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <string.h>
// #include <arpa/inet.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// constants:
#define TARGET_IP_ADDR "128.198.162.60"
#define TARGET_PORT 8080
#define SIZE 1024

//function to handle sending the file
void send_file( FILE *fp, char* fileName, int dest_socket)
{
    char data[SIZE] = {0};

    // send file name
    send(dest_socket, fileName, SIZE, 0);

    // send file contents
    while(fgets(data, SIZE, fp) != NULL)
    {
        if(send(dest_socket, data, sizeof(data), 0) == -1)
        {
            puts("[-]Error: sending the file failed. \n");
            exit(EXIT_FAILURE);
        }
        bzero(data, SIZE);
    }
}

int main()
{
    // variables
    struct sockaddr_in serverInfo;
    FILE *fp;
    char filePath[256];

  printf("Enter the file path: "); 
    fgets(filePath, 256, stdin);
  filePath[strcspn(filePath, "\n")] = '\0';
    // create socket using the socket() function
    int mySocket = socket(AF_INET, SOCK_STREAM, 0);

    // ensure socket was created, and terminate if it was not
    if(mySocket == -1)
    {
        puts("[-]Error: the socket couldn't be created \n");
        exit(EXIT_FAILURE);
    }
    puts("[+]Server socket was successfully created. \n");

    // setup struct with network info
    serverInfo.sin_family = AF_INET;
    serverInfo.sin_port = htons(TARGET_PORT);

    // convert IPv4 address from text to usable binary form
    if (inet_pton(serverInfo.sin_family, TARGET_IP_ADDR, &serverInfo.sin_addr) <= 0)
    {
        puts("[-]Error: The IP address is either invalid, or not supported \n");
        exit(EXIT_FAILURE);
    }

    // connect to server and test if connection was successful
    if((connect(mySocket, (struct sockaddr*)&serverInfo, sizeof(serverInfo))) < 0)
    {
        puts("[-]Error: Connection failed \n");
        exit(EXIT_FAILURE);
    }
    puts("[+]Connected to Server. \n");
    
    //open and read file to be sent
    fp = fopen(filePath, "r");
    if (fp == NULL) {
        puts("[-]Error in reading file. \n");
        exit(EXIT_FAILURE);
    }

    //send the file
    send_file(fp, filePath, mySocket); //need to pass file name, need to find way to pull file name from file path (see discord)
                                //send_file() updated to expect the above ^, this call needs to be updated though.
    puts("[+]File data was sent successfully. \n");

    //close socket
    puts("[+]Closing the socket connection. \n");
    close(mySocket);
    return 0;
}
