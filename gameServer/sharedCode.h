//File: sharedCode.h
// prj02 : 
// Author: CHIKE OKONTA
// CLASS: CSCI 312
// PROJECT 03: Headerfile
// Date: 10/19/2020
/*
*                   PURPOSE
*  Play rock Paper and Scissors using client server connections
*/
/*
*                   INSTRUCTIONS
* 1. Ensure ServerC.c, ServerG.c, sharedCode.h are all in the same directory
* 2. run the following line in terminal: make
* 4. run pwd to ensure makefile was successfully generated
* 5. run  the following command
*       ./ServerC
* 6. follow instructions on the screen
*/
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <netdb.h>

static const int MAXPENDING = 5; // Maximum outstanding connection requests

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
        servSock = socket(servAddr->ai_family, servAddr->ai_socktype, servAddr->ai_protocol);
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
