/*******************
 * Name: cardtest1.c
 * Author: Andrew Funk
 * Date: 02-09-19
 * Description: A unit test function for the dominion.c smithy card
 * 				effect. Original code outline is based on 
 * 				testUpdateCoins.c and cardtest4.c from OSU CS362 
 * 				course materials.
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

// This function compares the values in two gameState structs for the 
// second player. It returns 1 if differences are found, 0 if they are the 
// same.
int comparePlayerState(struct gameState *state1, struct gameState *state2)
{
	int i;

	//Check hand counts	
	if(state1->handCount[1] != state2->handCount[1] || 
		state1->deckCount[1] != state2->deckCount[1] ||
		state1->discardCount[1] != state2->discardCount[1])
	{
		return 1;
	}

	//Compare player deck, hand and discard pile
	for(i = 0; i < state1->handCount[1]; i++)
	{
		if(state1->hand[1][i] != state2->hand[1][i])
		{
			return 1;
		}
	}
	for(i = 0; i < state1->deckCount[1]; i++)
	{
		if(state1->deck[1][i] != state2->deck[1][i])
		{
			return 1;
		}
	}
	for(i = 0; i < state1->discardCount[1]; i++)
	{
		if(state1->discard[1][i] != state2->discard[1][i])
		{
			return 1;
		}
	}
	
	return 0;
}

// This function compares the values in two gameState structs for the 
// supply cards. It returns 1 if differences are found, 0 if they are 
// the same.
int compareSupplyState(struct gameState *state1, struct gameState *state2)
{
	int i;

	//Compare supply piles, ignore card type that was passed in
	for(i = 0; i < treasure_map; i++)
	{
		if(state1->supplyCount[i] != state2->supplyCount[i])
		{
			return 1;
		}
	}
	
	return 0;
}


int main() {
	int i, handPos;
	int seed = 5;
    int numPlayer = 2;
	int bonus = 0;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G, GCopy;

    printf ("TESTING Smithy Card:\n");

	//Clear the game state
	memset(&G, 23, sizeof(struct gameState)); 
	//Initialize a game
    initializeGame(numPlayer, k, seed, &G); 
	//Add a smithy to players hand
	gainCard(smithy, &G, 2, 0);

	//Make a copy of the original struct
	memcpy(&GCopy, &G, sizeof(struct gameState));

	//Find smithy position
	for(i = 0; i < G.handCount[0]; i++)
	{
		if(G.hand[0][i] == smithy)
		{
			handPos = i;
		}
	}

	cardEffect(smithy, 0, 0, 0, &G, handPos, &bonus);

	printf("TEST: Player receives 3 cards to hand.\n");
	// We only check for 2 more cards here because smithy is discarded
	assertTrue(G.handCount[0], GCopy.handCount[0] + 2);

	printf("TEST: 3 player cards come from own deck.\n");
	assertTrue(G.deckCount[0], GCopy.deckCount[0] - 3);

	printf("TEST: Smithy card no longer in hand after being played.\n");
	handPos = -1;
	for(i = 0; i < G.handCount[0]; i++)
	{
		if(G.hand[0][i] == smithy)
		{
			handPos = i;
		}
	}
	assertTrue(handPos, -1); 

	printf("TEST: Player 2 has no state change.\n");
	assertTrue(comparePlayerState(&G, &GCopy), 0);

	printf("TEST: Supply piles have no state change.\n");
	assertTrue(compareSupplyState(&G, &GCopy), 0);

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
