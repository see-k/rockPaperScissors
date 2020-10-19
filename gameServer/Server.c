// gameServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
// PULL TEST WAS SUCCESSFULL!!

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "Practical.h"

static const int MAXPENDING = 5; // Maximum outstanding connection requests

//Fuction: Set up TCP Server Socket
int SetupTCPServerSocket(const char* service) 
{
    // Construct the server address structure
    struct addrinfo addrCriteria; // Criteria for address match
    memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
    addrCriteria.ai_family = AF_UNSPEC; // Any address family
    addrCriteria.ai_flags = AI_PASSIVE; // Accept on any address/port
    addrCriteria.ai_socktype = SOCK_STREAM; // Only stream sockets
    addrCriteria.ai_protocol = IPPROTO_TCP; // Only TCP protocol

    struct addrinfo* servAddr; // List of server addresses
    int rtnVal = getaddrinfo(NULL, service, &addrCriteria, &servAddr);
    if (rtnVal != 0)
        DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));

    int servSock = -1;
    for (struct addrinfo* addr = servAddr; addr != NULL; addr = addr->ai_next) {
     // Create a TCP socket
        servSock = socket(servAddr->ai_family, servAddr->ai_socktype,
        servAddr->ai_protocol);
        if (servSock < 0)
            continue; // Socket creation failed; try next address
      
          // Bind to the local address and set socket to list
        if ((bind(servSock, servAddr->ai_addr, servAddr->ai_addrlen) == 0) && (listen(servSock, MAXPENDING) == 0)) 
        {
            // Print local address of socket
            struct sockaddr_storage localAddr;
            socklen_t addrSize = sizeof(localAddr);
            if (getsockname(servSock, (struct sockaddr*)&localAddr, &addrSize) < 0)
                DieWithSystemMessage("getsockname() failed");
            fputs("Binding to ", stdout);
            PrintSocketAddress((struct sockaddr*)&localAddr, stdout);
            fputc('\n', stdout);
            break; // Bind and list successful
            
        }
        close(servSock); // Close and try again
        servSock = -1;
    }
    // Free address list allocated by getaddrinfo()
     freeaddrinfo(servAddr);
     return servSock;
}

//Function Die With User Message
void DieWithUserMessage(const char* msg, const char* detail) {
      fputs(msg, stderr);
      fputs(": ", stderr);
      fputs(detail, stderr);
      fputc('\n', stderr);
      exit(1);

}

//Function Die With User Message
void DieWithSystemMessage(const char* msg) {
        perror(msg);
        exit(1);
}


//Fuction: HandleTCP Client
void HandleTCPClient(int clntSocket) {
    char buffer[BUFSIZE]; // Buffer for echo string

    // Receive message from client
    ssize_t numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
    if (numBytesRcvd < 0)
        DieWithSystemMessage("recv() failed");

    // Send received string and receive again until end of stream
    while (numBytesRcvd > 0)
    { // 0 indicates end of stream
        // Echo message back to client
        ssize_t numBytesSent = send(clntSocket, buffer, numBytesRcvd, 0);
        if (numBytesSent < 0)
            DieWithSystemMessage("send() failed");
        else if (numBytesSent != numBytesRcvd)
            DieWithUserMessage("send()", "sent unexpected number of bytes");

        // See if there is more data to receive
        numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
        if (numBytesRcvd < 0)
            DieWithSystemMessage("recv() failed");

    }
}

    
int main(int argc, char* argv[])
{

    if (argc != 2) // Test for correct number of arguments
        DieWithUserMessage("Parameter(s)", "<Server Port/Service>");

    char* service = argv[1]; // First arg: local port/service
    int servSock = SetupTCPServerSocket(service);
    if (servSock < 0)
        DieWithUserMessage("SetupTCPServerSocket() failed", "unable to establish");

    unsigned int childProcCount = 0; // Number of child processes

    //SERVER C
    for (;;) 
    { // Run forever
         // New connection creates a client socket
         int clntSock = AcceptTCPConnection(servSock);

         // Fork child process and report any errors
         pid_t processID = fork();

         if (processID < 0)
             DieWithSystemMessage("fork() failed");
         else if (processID == 0) 
         { // If this is the child process
             //Start ServerG
             close(servSock); // Child closes parent socketSet
             HandleTCPClient(clntSock);
             exit(0); // Child process terminates
         }
        
         printf("with child process: %d\n", processID);
         close(clntSock); // Parent closes child socket descriptor
         childProcCount++; // Increment number of child processes
        
             while (childProcCount) { // Clean up all zombies
             processID = waitpid((pid_t)-1, NULL, WNOHANG); // Non-blocking wait
             if (processID < 0) // waitpid() error?
                 DieWithSystemMessage("waitpid() failed");
             else if (processID == 0) // No zombie to wait on
                 break;
             else
                 childProcCount--; // Cleaned up after a child
            
        }
        
    }
    // NOT REACHED
       
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


