/*******************
 * Name: unittest3.c
 * Author: Andrew Funk
 * Date: 02-09-19
 * Description: A unit test function for the dominion.c function
 * 				supplyCount(). Original code outline is based on 
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

    printf ("TESTING supplyCount():\n");

	//Clear the game state
	memset(&G, 23, sizeof(struct gameState)); 
	//Initialize a game
    initializeGame(numPlayer, k, seed, &G); 
    
	printf("TEST: Kingdom card returns.\n");
	assertTrue(supplyCount(feast, &G), 10);

	printf("TEST: Treasure card returns.\n");
	assertTrue(supplyCount(silver, &G), 40);

	printf("TEST: Victory card returns.\n");
	assertTrue(supplyCount(province, &G), 8); 

	printf("TEST: Curse card returns.\n");
	assertTrue(supplyCount(curse, &G), 10); 

	printf("TEST: Card not in supply returns -1.\n");
	assertTrue(supplyCount(embargo, &G), -1); 

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
