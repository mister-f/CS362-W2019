/*******************
 * Name: randomtestcard2.c
 * Author: Andrew Funk
 * Date: 02-24-19
 * Description: A random test function for the dominion.c village card
 *              effect. Original code outline is based on
 *              testUpdateCoins.c and cardtest4.c from OSU CS362
 *              course materials.
 ******************/

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
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
// other player. It returns 1 if differences are found, 0 if they are the
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


int main() {
    int i, j, m, handPos, otherPlayer;
    int seed = 5;
    int numPlayer = 2;
    int bonus = 0;
    int randPlayer, randHandSize, randDeckSize, randDiscard, randAction;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G, GCopy;

    srand(time(NULL));

    // Iterate many random tests
    printf ("TESTING Village Card:\n");
    for(j = 0; j < 1000; j++)
    {
        // Randomly choose player
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

        // Randomly set decks, hands, discard piles
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

        // Set random player to go and put smithy in hand
        G.whoseTurn = randPlayer;
        G.hand[randPlayer][0] = village;
        handPos = 0;
		
		// Randomly set action number
		randAction = rand() % 10 + 1;
		G.numActions = randAction;

        // Make a copy of the original struct
        memcpy(&GCopy, &G, sizeof(struct gameState));

        // Play village
        cardEffect(village, 0, 0, 0, &G, handPos, &bonus);

        printf("\nRANDOM TEST SET %d:\n", j);
        printf("TEST: Player receives 1 card to hand.\n");
        // We only check for 'equal' cards here because smithy is discarded
        assertTrue(G.handCount[randPlayer], GCopy.handCount[randPlayer]);

        printf("TEST: Player card comes from own deck.\n");
        assertTrue(G.deckCount[randPlayer], GCopy.deckCount[randPlayer] - 1);

        printf("TEST: Village card no longer in hand after being played.\n");
        handPos = -1;
        if(G.hand[randPlayer][0] == village)
        {
            handPos = 0;
        }
        assertTrue(handPos, -1);

		printf("TEST: Village card is in discard pile after being played.\n");
		assertTrue(G.discardCount[randPlayer], GCopy.discardCount[randPlayer] + 1);

		printf("TEST: Number of actions increases by 2.\n");
		assertTrue(G.numActions, GCopy.numActions + 2);

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

