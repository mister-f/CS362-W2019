/*******************
 * Name: unittest1.c
 * Author: Andrew Funk
 * Date: 02-09-19
 * Description: A unit test function for the dominion.c function
 * 				isGameOver(). Original code outline is based on 
 * 				testUpdateCoins.c from OSU CS362 course materials.
 ******************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"

// Int to track failed tests
int failureCount = 0;

// Compares two values and checks if they are equal, equality indicates
// a passed test.
void assertTrue(int x, int y)
{
	if (x == y)
	{
		printf("Test passed.\n");
	}
	else
	{
		failureCount++;
		printf("Test failed.\n");
	}
}

int main() {
	int seed = 5;
    int numPlayer = 2;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G;

    printf ("TESTING isGameOver():\n");

	//Clear the game state
	memset(&G, 23, sizeof(struct gameState)); 
	//Initialize a game
    initializeGame(numPlayer, k, seed, &G); 
    
	printf("TEST: Initialized game state.\n");
	assertTrue(isGameOver(&G), 0);

	printf("TEST: Empty Province pile.\n");
	G.supplyCount[province] = 0;
	assertTrue(isGameOver(&G), 1);

	printf("TEST: Two empty supply piles.\n");
	G.supplyCount[province] = 8;
	G.supplyCount[curse] = 0;
	G.supplyCount[gold] = 0;
	assertTrue(isGameOver(&G), 0); 

	printf("TEST: Three empty supply piles (Tresure & Curse).\n");
	G.supplyCount[silver] = 0;
	assertTrue(isGameOver(&G), 1);

	printf("TEST: Three empty supply piles (Kingdom & Victory).\n");
	G.supplyCount[curse] = 1;
	G.supplyCount[gold] = 1;
	G.supplyCount[silver] = 1;
	G.supplyCount[mine] = 0;
	G.supplyCount[village] = 0;
	G.supplyCount[estate] = 0;
	assertTrue(isGameOver(&G), 1);

	if(failureCount == 0)
	{
    	printf("All tests passed!\n");
	}
	else
	{
		printf("%d test(s) failed.\n", failureCount);
	}

    return 0;
}
