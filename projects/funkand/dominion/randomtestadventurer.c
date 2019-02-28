/*******************
* Name: randomtestadventurer.c
* Author: Andrew Funk
* Date: 02-24-19
* Description: A random test function for the dominion.c adventurer card
*			effect. Original code outline is based on 
* 			testUpdateCoins.c and cardtest4.c from OSU CS362 
* 			course materials.
******************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "rngs.h"

// Int to track failed tests
int failureCount = 0;
int testCount = 0;

// Compares two values and checks if they are equal, equality indicates
// a passed test.
void assertTrue(int x, int y)
{
	testCount++;
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
// passed player. It returns 1 if differences are found, 0 if they are the 
// same.
int comparePlayerState(struct gameState *state1, struct gameState *state2, int player)
{
	int i;

	//Check hand counts	
	if(state1->handCount[player] != state2->handCount[player] || 
		state1->deckCount[player] != state2->deckCount[player] ||
		state1->discardCount[player] != state2->discardCount[player])
	{
		return 1;
	}

	//Compare player deck, hand and discard pile
	for(i = 0; i < state1->handCount[player]; i++)
	{
		if(state1->hand[player][i] != state2->hand[player][i])
		{
			return 1;
		}
	}
	for(i = 0; i < state1->deckCount[player]; i++)
	{
		if(state1->deck[player][i] != state2->deck[player][i])
		{
			return 1;
		}
	}
	for(i = 0; i < state1->discardCount[player]; i++)
	{
		if(state1->discard[player][i] != state2->discard[player][i])
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

// This function returns the total number of treasure cards in a players 
// hand.
int countTreasure(struct gameState *state, int player)
{
	int treasure = 0;
	int i;

	for(i = 0; i < state->handCount[player]; i++)
	{
		if(state->hand[player][i] == copper ||
			state->hand[player][i] == silver ||
			state->hand[player][i] == gold)
		{
			treasure++;
		}
	}

	return treasure;
}

int main() {
	int i, j, m, handPos, otherPlayer;
	int seed = 5;
    int numPlayer = 2;
	int bonus = 0;
	int randPlayer, randHandSize, randDeckSize, randDiscard;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G, GCopy;

	srand(time(NULL));    

	printf ("TESTING Adventurer Card:\n");

	for(j = 0; j < 1000; j++)
	{
		randPlayer = rand() % 2;
		if (randPlayer == 0)
		{
			otherPlayer = 1;
		}
		else
		{
			otherPlayer = 0;
		}

		//Clear the game state
		memset(&G, 23, sizeof(struct gameState)); 
	
		//Initialize a game
    	initializeGame(numPlayer, k, seed, &G); 

		for(i = 0; i < numPlayer; i++)
		{
			randDeckSize = rand() % MAX_DECK;
			G.deckCount[i] = randDeckSize;
			for(m = 0; m < randDeckSize; m++)
			{
				G.deck[i][m] = rand() % 18;
			}
			randHandSize = rand() % (MAX_DECK - randDeckSize);
			G.handCount[i] = randHandSize;
			for(m = 0; m < randHandSize; m++)
			{
				G.hand[i][m] = rand() % 18;
			}
			randDiscard = rand() % (MAX_DECK - randDeckSize - randHandSize);
			G.discardCount[i] = randDiscard;
			for(m = 0; m < randDiscard; m++)
			{
				G.discard[i][m] = rand() % 18;
			}
		}
	
		// Set random player to go and put adventurer in hand
		G.whoseTurn = randPlayer;	
		G.hand[randPlayer][0] = adventurer;
		handPos = 0;

		//Make a copy of the original struct
		memcpy(&GCopy, &G, sizeof(struct gameState));

		cardEffect(adventurer, 0, 0, 0, &G, handPos, &bonus);

		printf("\nRANDOM TEST SET %d:\n", j);
		printf("TEST: Player receives 2 treasures to hand.\n");
		assertTrue(countTreasure(&G, randPlayer), countTreasure(&GCopy, randPlayer) + 2);

		printf("TEST: Treasure cards come from own deck.\n");
		assertTrue(G.deckCount[randPlayer] + G.discardCount[randPlayer], GCopy.deckCount[randPlayer] + G.discardCount[randPlayer] - 2);

		printf("TEST: Adventurer card no longer in hand after being played.\n");
		handPos = -1;
		if(G.hand[randPlayer][0] == adventurer)
		{
			handPos = 0;
		}
		assertTrue(handPos, -1); 
	
		printf("TEST: Drawn non-treasure cards are properly discarded.\n");
		assertTrue(G.deckCount[randPlayer] + G.handCount[randPlayer] + G.discardCount[randPlayer], GCopy.deckCount[randPlayer] + GCopy.handCount[randPlayer]);	
	
		printf("TEST: Other player has no state change.\n");
		assertTrue(comparePlayerState(&G, &GCopy, otherPlayer), 0);
	
		printf("TEST: Supply piles have no state change.\n");
		assertTrue(compareSupplyState(&G, &GCopy), 0);
	}
	
	printf("\nTotal tests run: %d\n", testCount);
	printf("Test(s) passed: %d\n", testCount - failureCount);
	printf("Test(s) failed: %d\n", failureCount);

    return 0;
}
