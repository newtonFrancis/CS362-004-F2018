//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//     NAME: Benjamin Redmon
//     TEST: Random Card Test
// FUNCTION: Adventurer
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

int checkAdventurer(struct gameState*, int, int, int*);

int main()
{
  struct gameState G;
  int p,
      h,
      bonus = -1,
      test,
      pass = 0, fail = 0, code = 0,
      idx,
      path;

  SelectStream(2);
  PutSeed(3);

  // run two-thousand random tests
  for (int i = 0; i < 2000; i++)
  {
    // set every bit in gameState G to a random character
    for (int j = 0; j < sizeof(struct gameState); j++)
      ((char*)&G)[j] = floor(Random() * 256);

    // set some parameters specific to testing steward
    G.numPlayers = 2;           // set number of players to 2
    p = floor(Random() * 2);    // set player to 0 or 1
    G.whoseTurn = p;            // set player turn

    // set deck and discard count to max deck size
    G.deckCount[p] = floor(Random() * MAX_DECK);
    G.discardCount[p] = floor(Random() * MAX_DECK);

    // set hand count to max hand size
    G.handCount[p] = floor(Random() * MAX_HAND);

    // set hand count to at least one -- to hold the adventurer card
    if (G.handCount[p] < 1)
      G.handCount[p] = 1;

    // randomly select a hand position and set to adventurer
    h = floor(Random() * G.handCount[p]);
    G.hand[p][h] = adventurer;


    path = G.deckCount[p] % 3;

    // even deck count -- add five to deck
    if (path == 0)
    {
      //printf("0\n");

      G.discardCount[p] = 50;

      // deck is greater than or equal to five
      if (G.deckCount[p] >= 5)
      {
        for (int i = 0; i < 5; i++)
        {
          idx = floor(Random() * G.deckCount[p]);
          G.deck[p][idx] = floor(Random() * 3) + copper;
        }

      }
      // deck is less than five
      else
      {
        G.deckCount[p] = 5;

        for (int i = 0; i < 5; i++)
          G.deck[p][i] = floor(Random() * 3) + copper;
      }
    }
    // odd deck count -- add five to discard
    else if (path == 1)
    {
      //printf("00\n");

      G.deckCount[p] = 50;

      // deck is greater than or equal to five
      if (G.discardCount[p] >= 5)
      {
        for (int i = 0; i < 5; i++)
        {
          idx = floor(Random() * G.discardCount[p]);
          G.discard[p][idx] = floor(Random() * 3) + copper;
        }

      }
      // deck is less than five
      else
      {
        G.discardCount[p] = 5;

        for (int i = 0; i < 5; i++)
          G.discard[p][i] = floor(Random() * 3) + copper;
      }
    }
    else
    {
      //printf("000\n");

      G.discardCount[p] = 1;
      G.deckCount[p] = 1;

      for (int i = 0; i < 1; i++)
      {
        G.discard[p][i] = floor(Random() * 3) + copper;
        G.deck[p][i] = floor(Random() * 3) + copper;
      }

    }


    // run the checkSmithy test -- returns greater than 0 if failed
    test = checkAdventurer(&G, p, h, &bonus);

    // check if adventurer test failed
    if (test > 0)
    {
      fail++;
      code = test;
    }
    else
      pass++;
  }

  // print the results of the tests
  printf("PASSED: %d Tests\n", pass);

  if (fail == 0)
    printf("FAILED: %d Tests\n", fail);
  else
    printf("FAILED: %d Tests - Code %d\n", fail, code);


  return 0;
}

int checkAdventurer(struct gameState *post, int p, int handPos, int* bonus)
{
  struct gameState pre;

  // create a copy of the current game state
  memcpy(&pre, post, sizeof(struct gameState));

  //printf("handPos: %d, deckCount: %d\n", post->hand[post->whoseTurn][handPos], post->deckCount[p]);

  // run card effect for the smithy card
  cardEffect(post->hand[post->whoseTurn][handPos], -1, -1, -1, post, handPos, bonus);

  // set expected conditions after running cardEffect
  pre.handCount[p] = pre.handCount[p] + 2;  // hand count increased by two


  // new card #1 is treasure
  if (post->hand[p][pre.handCount[p] - 1] < copper || post->hand[p][pre.handCount[p] - 1] > gold)
  {
    // not treasure
    return 1;
  }
  else
  {
    pre.hand[p][pre.handCount[p] - 1] = post->hand[p][pre.handCount[p] - 1];
  }

  // new card #2 is treasure
  if (post->hand[p][pre.handCount[p] - 2] < copper || post->hand[p][pre.handCount[p] - 2] > gold)
  {
    // not treasure
    return 2;
  }
  else
  {
    pre.hand[p][pre.handCount[p] - 2] = post->hand[p][pre.handCount[p] - 2];
  }

  // copy deck and discard counts from adding treasure
  pre.deckCount[p] = post->deckCount[p];
  pre.discardCount[p] = post->discardCount[p];

  // copy deck and discard to reflect shuffled cards during play
  memcpy(pre.discard[p], post->discard[p], sizeof(int) * MAX_DECK);
  memcpy(pre.deck[p], post->deck[p], sizeof(int) * MAX_DECK);



  if (memcmp(&pre, post, sizeof(struct gameState)) != 0)
  {
    if (pre.numPlayers != post->numPlayers)
      printf("numPlayers: %d vs. %d\n", pre.numPlayers, post->numPlayers);

    for (int i = curse; i <= treasure_map; i++)
    {
      if (pre.supplyCount[i] != post->supplyCount[i])
        printf("supplyCount: %d vs. %d\n", pre.supplyCount[i], post->supplyCount[i]);
    }

    for (int i = curse; i <= treasure_map; i++)
    {
      if (pre.embargoTokens[i] != post->embargoTokens[i])
        printf("embargoTokens: %d vs. %d\n", pre.embargoTokens[i], post->embargoTokens[i]);
    }

    if (pre.outpostPlayed != post->outpostPlayed)
      printf("outpostPlayed: %d vs. %d\n", pre.outpostPlayed, post->outpostPlayed);

    if (pre.outpostTurn != post->outpostTurn)
      printf("outpostTurn: %d vs. %d\n", pre.outpostTurn, post->outpostTurn);

    if (pre.phase != post->phase)
      printf("phase: %d vs. %d\n", pre.phase, post->phase);

    if (pre.numActions != post->numActions)
      printf("numActions: %d vs. %d\n", pre.numActions, post->numActions);

    if (pre.coins != post->coins)
      printf("coins: %d vs. %d\n", pre.coins, post->coins);

    if (pre.numBuys != post->numBuys)
      printf("numBuys: %d vs. %d\n", pre.numBuys, post->numBuys);

    if (pre.coins != post->coins)
      printf("coins: %d vs. %d\n", pre.coins, post->coins);

    if (pre.numBuys != post->numBuys)
      printf("numBuys: %d vs. %d\n", pre.numBuys, post->numBuys);

    for (int i = 0; i < post->numPlayers; i++)
    {
      if (pre.handCount[i] != post->handCount[i])
        printf("handCount: %d vs. %d\n", pre.handCount[i], post->handCount[i]);
      else
      {
        for (int j = 0; j < MAX_HAND; j++)
        {
          if (pre.hand[i][j] != post->hand[i][j])
            printf("hand[%d][%d]: %d vs. %d  (%d)\n", i, j, pre.hand[i][j], post->hand[i][j], handPos);
        }
      }

      if (pre.discardCount[i] != post->discardCount[i])
      {
        printf("discardCount: %d vs. %d\n", pre.discardCount[i], post->discardCount[i]);
        printf("deckCount: %d vs. %d\n", pre.deckCount[i], post->deckCount[i]);
      }
      else
      {
        for (int j = 0; j < MAX_DECK; j++)
        {
          if (pre.discard[i][j] != post->discard[i][j])
            printf("discard[%d][%d]: %d vs. %d\n", i, j, pre.discard[i][j], post->discard[i][j]);
        }
      }

      if (pre.deckCount[i] != post->deckCount[i])
      {
        printf("deckCount: %d vs. %d\n", pre.deckCount[i], post->deckCount[i]);
      }
      else
      {
        for (int j = 0; j < MAX_DECK; j++)
        {
          if (pre.deck[i][j] != post->deck[i][j])
            printf("deck[%d][%d]: %d vs. %d\n", i, j, pre.deck[i][j], post->deck[i][j]);
        }
      }
    }


    if (pre.playedCardCount != post->playedCardCount)
    {
      printf("playedCardCount: %d vs. %d\n", pre.playedCardCount, post->playedCardCount);
    }
    else
    {
      for (int j = 0; j < MAX_DECK; j++)
      {
        if (pre.playedCards[j] != post->playedCards[j])
        {
          printf("playedCards[%d]: %d vs. %d\n", j, pre.playedCards[j], post->playedCards[j]);
        }
      }
    }

    printf("MEMCMP: %d (%d)\n", memcmp(&pre, post, sizeof(struct gameState)), handPos);
    
    return 5;
  }

  return 0;

}
