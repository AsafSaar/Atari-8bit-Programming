#include <atari.h>
#include <stdio.h>
#include <stdlib.h>
#include <peekpoke.h>

unsigned char userNum, loopCount, num, i, found, atariClock;

void printRandoms() 
{
	// Creating true random numbers using Atari's internal clock
	atariClock = PEEK(0x14);
	srand(atariClock);
    num =  rand() % 100;

    // Checking gamer guess with Atari's number
    if (num == userNum) {
    	printf("Atari's number is: %d\n\n", num);
    	printf("You are correct :-)\n\n");

    	printf("You won. Game Over\n");

    	//Endless loop to keep te app running
		while (1) {
		}
    }
    else {
    	printf("Wrong! Atari's number is: %d\n\n", num);
    }
}

// Get game guyess
int getNumberFromUser() {
	printf("What's your guess? ");
	scanf("%i", &i);
	return(i);
}

void main(void)
{
	loopCount = 0;

	printf("\nLet's play a small game!\n");
	printf("You have 3 shots to guess a\nnumbers from 1 - 100\n\n\n");

	while(loopCount <= 2) {
		userNum = getNumberFromUser();
		printRandoms();
		loopCount++;
	}

   	printf("You lost. Game Over\n");

   	//Endless loop to keep te app running
	while (1) {
	}

}