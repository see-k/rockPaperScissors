# rockPaperScissors
1.	Client-Server: 
 
a.	Server:  After the server process executes a fork system call to create a child process to handle a particular client connection, have the child process exec another program, ServerG, to perform the server tasks for playing the game.  That is, the server will consist for two programs:  ServerC and ServerG.  ServerC is responsible for accepting the initial connection and establishing each child process based on the ServerG program.  ServerC must pass the socket descriptor to ServerG.  In addition, ServerC must continue to accept connections and monitor the termination of all child processes.  The textbook:  TCP/IP Sockets in C by Donahoo and Calvert in Section 6.4.1 contains relevant ideas.  Appropriate console messages should be issued by ServerC to document the occurrence of significant events (new connection accepted, child created, child died, etc.).   
 
b.	Client:  The client is responsible for all communication with the human user who is playing the game.  This includes such things as prompting for user input, displaying the status of the game, displaying the server’s 
“move”, displaying error messages, etc. 
 
c.	Shared code:  Create a C/C++-module that contains any common subroutines that are used by both the client and server.  Link the resulting code into both the server and client programs.  Client and server must utilize these routines.  In addition, if there are any shared structure definitions or symbol definitions, define these in a .h file that both client and server use. 
 
d.	System call usage:  Use each system call in an appropriate manner; read the man pages for the system calls you use. 
 
2.	The Game:  Rock-Paper-Scissors as described below: 
 
Play best n out of m rounds of the game:  Rock-Paper-Scissors.  n and m are randomly selected to be:  2 out of 3, 3 out of 5, 4 out of 7, or 5 out of 9.  In each round of this children’s game, the two players simultaneously display symbols for:  rock, paper or scissor.  The winner is determined as follows: Paper beats rock, rock beats scissors, scissors beats paper; all other combinations are a draw. 
