/* 
 * Example program of socket use within a client. 
 * 
 * References: 
* Linux man pages for server 199.17.28.75: socket, bind,   * listen, accept, ip(7). 
 * Donahoo and Calvert, TCP/IP Sockets, pp. 11 – 33. 
 * Robins and Robins, UNIX Systems Programming, pp. 630 - 642.  */ 
 
#include 	<errno.h> 
#include 	<stdio.h> 
#include 	<stdlib.h> 
#include 	<unistd.h> 
#include 	"soc.h" 
#include	<string.h>
#include	<sys/types.h>
#include	<unistd.h>
#include	<stdbool.h>
#include	<sys/wait.h>

#define	BUFL	100

int main (int argc, char *argv[]) 
{ 
 	int err; 
 	int cSocket; 
 	int choice;
 	char Buf0[BUFL], Buf1[BUFL], Buf2[BUFL], Buf3[BUFL];
 	bool done = false;
 	struct sockaddr_in sAddr; 
 	struct sockaddr_in cAddr; 
 	int cSocLen; 
 	memset (&cAddr, 0, sizeof (struct sockaddr_in));  
 	cAddr.sin_family = AF_INET; 
 	cAddr.sin_port = htons (SERVERPORT); 
 	cAddr.sin_addr.s_addr = inet_addr(SERVERIP); 
 	
 	cSocket = socket ( AF_INET, SOCK_STREAM, 0); // AF_INET  
 	if (cSocket == -1) 
 	{ 
 		perror ("socClient: socket creation failed");  exit (1); 
 	} 
 	
 	err = connect (cSocket, (struct sockaddr *)&cAddr,  sizeof(struct sockaddr_in)); 
 	
 	if (err == -1) 
 	{ 
 		perror ("socClient: connect failed"); 
 		exit (2); 
 	} 
 	
 	// Send message to server. 
 	
 	//err = send (cSocket, "Connection made!\n", 17, 0);  
 	//printf ("socClient: number of bytes sent to server: %d\n", err);
 	do
	{
		err = recv(cSocket, Buf0, 17, MSG_WAITALL);
	} while (err == -1);
	printf("%c\n", Buf0); 
	
 	while(!done)
	 {
	 	
	 	printf ("Please enter your move.  Type '1' for Rock, type '2' for Paper, and type '3' for Scissors. \n");
	 	scanf("%d", &choice);
	 	if (choice == 1)
	 	{
	 		err = send(cSocket, "rock", 17, 0);
	 		do
	 		{
	 			err = recv(cSocket, Buf1, 17, MSG_WAITALL);
	 		} while (err == -1);
	 		printf("%c", Buf1);  //Something like "Opponent chose scissors.  Exchange won."
	 		do
	 		{
	 			err = recv(cSocket, Buf2, 17, MSG_WAITALL);
	 		} while (err == -1);
	 		if (Buf2 == "done")
	 		{
	 			done = true;
	 		}
	 		else
	 		{
	 			printf("Next round:  \n");
	 		}
	 	}
	 	else if (choice == 2)
	 	{
	 		err = send(cSocket, "paper", 17, 0);
	 		do
	 		{
	 			err = recv(cSocket, Buf1, 17, MSG_WAITALL);
	 		} while (err == -1);
	 		printf("%c", Buf1);  //Something like "Opponent chose scissors.  Exchange lost."
	 		do
	 		{
	 			err = recv(cSocket, Buf2, 17, MSG_WAITALL);
	 		} while (err == -1);
	 		if (Buf2 == "done")
	 		{
	 			done = true;
	 		}
	 		else
	 		{
	 			printf("Next round:  \n");
	 		}
	 	}
	 	else if (choice == 3)
	 	{
	 		err = send(cSocket, "scissors", 17, 0);
	 		do
	 		{
	 			err = recv(cSocket, Buf1, 17, MSG_WAITALL);
	 		} while (err == -1);
	 		printf("%c", Buf1);  //Something like "Opponent chose scissors.  Exchange is a tie."
	 		do
	 		{
	 			err = recv(cSocket, Buf2, 17, MSG_WAITALL);
	 		} while (err == -1);
	 		if (Buf2 == "done")
	 		{
	 			done = true; //End loop
	 		}
	 		else
	 		{
	 			printf("Next round:  \n");
	 		}
	 	}
	 	else
	 	{
	 		printf("ERROR. \n");
	 	}
	 	//Receive game status from server
	 	//Display server move
	 	//Done if server declares done
 	}
 	/*do
	{
		err = recv(cSocket, Buf3, 17, MSG_WAITALL); //Declare winner
		printf("%c\n", Buf3);
	} while (err == -1);*/
 	exit (0); 
} 

