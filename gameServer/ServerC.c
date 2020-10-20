//File: ServerC.c
// prj02 : This file contains the 'main' function. Program execution begins and ends there.
// Author: CHIKE OKONTA
// CLASS: CSCI 312
// PROJECT 02: Implementation file (ServerC.c)
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
#include "sharedCode.h"
#include "soc.h"


int AcceptTCPConnection(int servSock) 
{
     struct sockaddr_storage clntAddr; // Client address
     // Set length of client address structure (in-out parameter)
     socklen_t clntAddrLen = sizeof(clntAddr);
    
         // Wait for a client to connect
     int clntSock = accept(servSock, (struct sockaddr*)&clntAddr, &clntAddrLen);
     if (clntSock < 0)
         DieWithSystemMessage("accept() failed");
    
         // clntSock is connected to a client!
        
     fputs("Handling client ", stdout);
     PrintSocketAddress((struct sockaddr*)&clntAddr, stdout);
     fputc('\n', stdout);
    
     return clntSock;
    
}


//Fuction: HandleTCP Client
void HandleTCPClient(int clntSocket) {

    //Launch ServerG
    int  err =  execl("./ServerG", "ServerG", clntSocket, (char *)NULL);
    if (err == -1)
    {
        printf("Failed to launch ServerG");
        exit(1);
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
        
         while (childProcCount)
         { // Clean up all zombies
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


    //char buffer[BUFSIZE]; // Buffer for echo string

    //// Receive message from client
    //ssize_t numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
    //if (numBytesRcvd < 0)
    //    DieWithSystemMessage("recv() failed");

    //// Send received string and receive again until end of stream
    //while (numBytesRcvd > 0)
    //{ // 0 indicates end of stream
    //    // Echo message back to client
    //    ssize_t numBytesSent = send(clntSocket, buffer, numBytesRcvd, 0);
    //    if (numBytesSent < 0)
    //        DieWithSystemMessage("send() failed");
    //    else if (numBytesSent != numBytesRcvd)
    //        DieWithUserMessage("send()", "sent unexpected number of bytes");

    //    // See if there is more data to receive
    //    numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
    //    if (numBytesRcvd < 0)
    //        DieWithSystemMessage("recv() failed");
