/*******************
 * Name: unittest2.c
 * Author: Andrew Funk
 * Date: 02-09-19
 * Description: A unit test function for the dominion.c function
 * 				fullDeckCount(). Original code outline is based on 
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
	int i;
	int seed = 5;
    int numPlayer = 2;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G;

    printf ("TESTING fullDeckCount():\n");

	//Clear the game state
	memset(&G, 23, sizeof(struct gameState)); 
	//Initialize a game
    initializeGame(numPlayer, k, seed, &G); 
    
	printf("TEST: P1 coppers, initialized game state.\n");
	assertTrue(fullDeckCount(0, copper, &G), 7);

	printf("TEST: P2 coppers, initialized game state.\n");
	assertTrue(fullDeckCount(1, copper, &G), 7);

	printf("TEST: Cards located only in hand.\n");
	for(i = 0; i < MAX_HAND; i++)
	{
		G.hand[0][i] = silver;
		G.handCount[0]++;
	}
	assertTrue(fullDeckCount(0, silver, &G), MAX_HAND); 

	printf("TEST: Cards located only in discard.\n");
	for(i = 0; i < MAX_DECK; i++)
	{
		G.discard[0][i] = gold;
		G.discardCount[0]++;
	}
	assertTrue(fullDeckCount(0, gold, &G), MAX_DECK); 

	printf("TEST: Cards located in hand, deck, & discard.\n");
	for(i = 0; i < 5; i++)
	{
		G.hand[1][i] = copper;
		G.handCount[1]++;
		G.discard[1][i] = copper;
		G.discardCount[1]++;
	}
	assertTrue(fullDeckCount(1, copper, &G), 17); 

	printf("TEST: No cards of type in any location.\n");
	assertTrue(fullDeckCount(0, mine, &G), 0);

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
