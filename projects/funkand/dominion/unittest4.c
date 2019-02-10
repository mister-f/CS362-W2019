/*******************
 * Name: unittest4.c
 * Author: Andrew Funk
 * Date: 02-09-19
 * Description: A unit test function for the dominion.c function
 * 				gainCard(). Original code outline is based on 
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

    printf ("TESTING gainCard():\n");

	//Clear the game state
	memset(&G, 23, sizeof(struct gameState)); 
	//Initialize a game
    initializeGame(numPlayer, k, seed, &G); 
   
	printf("TEST: Can gain card to deck.\n");
	gainCard(mine, &G, 1, 0);
	assertTrue(G.deck[0][G.deckCount[0] - 1], mine);

	printf("TEST: Can gain card to hand.\n");
	gainCard(mine, &G, 2, 0);
	assertTrue(G.hand[0][G.handCount[0] - 1], mine);

	printf("TEST: Can gain card to discard.\n");
	gainCard(mine, &G, 0, 0);
	assertTrue(G.discard[0][G.discardCount[0] - 1], mine);

	printf("TEST: Can't gain from empty supply.\n");
	G.supplyCount[smithy] = 0;
	assertTrue(gainCard(smithy, &G, 0, 0), -1); 

	printf("TEST: Can't gain from card not in game.\n");
	assertTrue(gainCard(embargo, &G, 0, 0), -1); 

	printf("TEST: Gaining card reduces remaining supply.\n");
	gainCard(feast, &G, 0, 0);
	assertTrue(G.supplyCount[feast], 9);

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
