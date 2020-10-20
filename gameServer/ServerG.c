/*
ServerG.c
Communicates with client, getting the user's choice for Rock Paper Scissors
and comparing it to a random CPU, sending the client what to output and
when to stop running. Keeps running until the Player or CPU won n rounds.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sharedCode.h"
#include "soc.h"

#define BUFFERSIZE 1024

void clearBuffer(char *input, int length)
{
    for(int i=0;i<length;i++)
        *(input+i) = '\0';
}

int main(int argc, char* argv[])
{
    int cSocket;
    int err;
    int score[2] = {0, 0}; //score[0] is player score, 1 is CPU score
    int choice, cpuChoice; //0 for rock, 1 for paper, 2 for scissors
    int point; //0 for tie, 1 for player gets point, 2 for CPU gets point
    int n; //first to n points wins
    char enumb1[3][9] = {"rock", "paper", "scissors"}; //0 = rock etc
    char enumb2[3][7] = {"nobody", "Player", "CPU"}; //0 = nobody gets a point etc
    char rbuffer[BUFFERSIZE+1]; //read buffer
    char wbuffer[BUFFERSIZE+1]; //write buffer

    //Generate n, the number of points to win
    srand(time(NULL));
    n = rand() % 4 + 2; //n = {2,3,4,5} aka best of {3,5,7,9}

    //Launch game:
    clearBuffer(wbuffer, BUFFERSIZE);
    sprintf(wbuffer, "Game Started\n First to %d points wins!\nEnter your move (r p s): ", n);
    do {
//FIX: CHECK THIS SOCKET NAME
        err = send(cSocket, wbuffer, strlen(wbuffer) + 1, 0); //send the starting message
    } while (err < 0);
    
    while ((score[0] < n ) && (score[1] < n)) { //while nobody has enough points to win, keep playing
        clearBuffer(wbuffer, BUFFERSIZE);
        
        //get choice from user:
        do {
            clearBuffer(rbuffer, BUFFERSIZE);
            err = recv(cSocket, rbuffer, strlen(rbuffer) + 1, 0);
        } while (err < 0);
        choice = atoi(rbuffer);
        
        //make sure user's choice is between 0 and 2:
        if (choice < 0 || choice > 3)
            sprintf(wbuffer, "Error receiving message.\n");
        else if (choice == 3) { //termination flag
//FIX: ADD PROPER TERMINATION
            exit(3);
        }
        else { //choice is between 0 and 2, figure out who gets the point and if there's a winner:
            //get random cpu choice:
            cpuChoice = rand() % 3;
            point = abs(choice - cpuChoice); //calculates who should get the point
            if (point > 0) //if not a tie, increment score
                score[point-1]++;
            
            sprintf(wbuffer, "Player chose %s, CPU chose %s,", enumb1[choice], enumb1[cpuChoice]);
            sprintf(wbuffer, "%s, point goes to %s.\n", wbuffer, enumb2[point]);
            sprintf(wbuffer, "%sPlayer score: %d \tCPU score: %d\n", wbuffer, score[0], score[1]);
            
            //if still not enough points, then send the message and loop. Else send it later.
            if (point == 0 || score[point-1] < n) {
                sprintf(wbuffer, "%sEnter your move (r p s): ", wbuffer);
                do {
                    err = send(cSocket, wbuffer, strlen(wbuffer) + 1, 0);
                } while (err < 0);
                
                clearBuffer(wbuffer, BUFFERSIZE);
                sprintf(wbuffer, "this"); //extra message so client knows to keep getting user input
                do {
                    err = send(cSocket, wbuffer, strlen(wbuffer) + 1, 0);
                } while (err < 0);
            }
        }
    }
    //A winner has been decided. Find winner and print last message:
    
    if (score[0] > score[1]) //Player won!
        sprintf(wbuffer, "%sYou won!!", wbuffer);
    else //CPU won.
        sprintf(wbuffer, "%sCPU won!!", wbuffer);
    do {
        err = send(cSocket, wbuffer, strlen(wbuffer) + 1, 0);
    } while (err < 0);
    
    clearBuffer(wbuffer, BUFFERSIZE);
    sprintf(wbuffer, "done"); //extra message so client knows to stop getting user input
    do {
        err = send(cSocket, wbuffer, strlen(wbuffer) + 1, 0);
    } while (err < 0);
    
//FIX: maybe close sockets like we do pipes?
    
    return 0;
}