//File: sharedCode.h
// prj02 : 
// Author: CHIKE OKONTA
// CLASS: CSCI 312
// PROJECT 03: Implementation file (ServerG.c)
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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sharedCode.h"


#define bufferSize 1024
 
int main(int argc, char* argv[])
{
    int R = 1;
    int P = 2;
    int S = 3;
    int i;
    int p1Score = 0;
    int p2Score = 0;
    int choice;
    char p1response[bufferSize];

    //Generate n = number of points required to win
    srand(time(NULL));
    int n = rand() % 4 + 2;

    //Launch game
    printf("Game Started\n First to %d points wins the match!\n ", n);
    printf("Rock=1 , Paper= 2, and Scissors= 3\n");

    while ((p1Score < n ) && (p2Score < n)) {
        printf("Enter your choice Player 1:");
//change to communicate with client:
        scanf("%d", &choice);
        int Player2 = rand() % 3 + 1;
        if (choice == 1) {
            if (Player2 == 1) {
                printf("Draw\n");
            }
            if (Player2 == 2) {
                printf("Player 2 Wins!\n");
                p2Score = p2Score + 1;
            }
            if (Player2 == 3) {
                printf("Player 1 Wins\n");
                p1Score = p1Score + 1;
            }
        }
        else  if (choice == 2) {
            if (Player2 == 2) {
                printf("Draw\n");
            }
            if (Player2 == 3) {
                printf("Player 1 Wins!\n");
                p1Score = p1Score + 1;
            }
            if (Player2 == 1) {
                printf("Player 2 Wins!\n");
                p2Score = p2Score + 1;
            }
        }
        else if (choice == 3) {
            if (Player2 == 3) {
                printf("Draw\n");
            }
            if (Player2 == 2) {
                printf("Player 2 Wins!\n");
                p2Score = p2Score + 1;
            }
            if (Player2 == 1) {
                printf("Player 1 Wins!\n");
                p1Score = p1Score + 1;

            }
        }
        else {
            printf("Wrong Answer\n");
        }

    }
    if (p2Score > p1Score) {
        printf("Player 2 wins %d to %d\n", p2Score, p1Score);
    }
    else if (p2Score < p1Score) {
        printf("Player 1 wins %d to %d\n", p1Score, p2Score);
    }
    else if (p2Score = p1Score) {
        printf("No winner it is a draw!\n");
    }



    return 0;
}
