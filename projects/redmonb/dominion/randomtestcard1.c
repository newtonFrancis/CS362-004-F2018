//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// NAME: Benjamin Redmon
// TEST: Random Card Test #1
// CARD: Smithy
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

int checkSmithy(struct gameState*, int, int, int*);

int main()
{
  struct gameState G;
  int p, h, bonus = -1, test, pass = 0, fail = 0, code = 0;

  SelectStream(2);
  PutSeed(3);

  // run two-thousand random tests
  for (int i = 0; i < 2000; i++)
  {
    // set every bit in gameState G to a random character
    for (int j = 0; j < sizeof(struct gameState); j++)
      ((char*)&G)[j] = floor(Random() * 256);

    // set some parameters specific to testing smithy
    G.numPlayers = 2;           // set number of players to 2
    p = floor(Random() * 2);    // set player to 0 or 1
    G.whoseTurn = p;            // set player turn
    //G.playedCardCount = 0;      // no cards played

    // set deck, discard, and played count to max deck size
    G.deckCount[p] = floor(Random() * MAX_DECK);
    G.discardCount[p] = floor(Random() * MAX_DECK);
    G.playedCardCount = floor(Random() * MAX_DECK);

    // set hand count to max hand size
    G.handCount[p] = floor(Random() * MAX_HAND);

    // set hand count to at least one -- to hold the smithy card
    if (G.handCount[p] == 0)
      G.handCount[p]++;

    // randomly select a hand position and set to smithy
    h = floor(Random() * G.handCount[p]);
    G.hand[p][h] = smithy;

    // run the checkSmithy test -- returns greater than 0 if failed
    test = checkSmithy(&G, p, h, &bonus);

    // check if smithy test failed
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

int checkSmithy(struct gameState *post, int p, int handPos, int* bonus)
{
  struct gameState pre;

  // create a copy of the current game state
  memcpy(&pre, post, sizeof(struct gameState));

  // run card effect for the smithy card
  cardEffect(post->hand[post->whoseTurn][handPos], -1, -1, -1, post, handPos, bonus);

  //printf("MEMCMP A: %d\n", memcmp(&pre, post, sizeof(struct gameState)));

  // set expected conditions after running cardEffect
  pre.playedCardCount++;                    // smithy discarded
  pre.handCount[p] = pre.handCount[p] + 2;  // hand count increased by two

  // enough cards in deck to draw three
  if (pre.deckCount[p] >= 3)
  {
    // subtract the drawn cards
    pre.deckCount[p] = pre.deckCount[p] - 3;
  }
  // not enough cards in deck to draw three
  else if (pre.deckCount[p] < 3)
  {
    // account for re-shuffled discarded deck when empty
    pre.deckCount[p] = pre.discardCount[p] - 3 + pre.deckCount[p];

    // copy deck and discard to reflect shuffled cards during play
    memcpy(pre.discard[p], post->discard[p], sizeof(int) * pre.discardCount[p]);
    memcpy(pre.deck[p], post->deck[p], sizeof(int) * (pre.deckCount[p] + 3));

    pre.discardCount[p] = 0;
  }

  // draw cards from deck into hand
  pre.hand[p][pre.handCount[p] - 2] = pre.deck[p][pre.deckCount[p] + 2];
  pre.hand[p][pre.handCount[p] - 1] = pre.deck[p][pre.deckCount[p] + 1];
  pre.hand[p][pre.handCount[p] - 0] = pre.deck[p][pre.deckCount[p] + 0];

  // set played card to smithy
  pre.playedCards[pre.playedCardCount - 1] = smithy;

  // remove smithy from hand
  pre.hand[p][handPos] = pre.hand[p][pre.handCount[p]];
  pre.hand[p][pre.handCount[p]] = -1;

  // compare post conditions with expected conditions
  // same number of cards in hand
  if (pre.handCount[p] != post->handCount[p])
  {
    //printf("FAILED: Hand Count\n");
    return 1;
  }
  // same number of cards in deck
  else if (pre.deckCount[p] != post->deckCount[p])
  {
    //printf("FAILED: Deck Count\n");
    return 2;
  }
  // same number of cards played
  else if (pre.playedCardCount != post->playedCardCount)
  {
    //printf("FAILED: Played Count\n");
    return 3;
  }
  // played card was a smithy
  else if (pre.playedCards[pre.playedCardCount - 1] != post->playedCards[post->playedCardCount - 1])
  {
    //printf("FAILED: Smithy Played\n");
    return 4;
  }

  // copy hand to reflect cards drawn during play
  // for small percentage of situations when deck is shuffled mid-draw
  memcpy(pre.hand[p], post->hand[p], sizeof(int) * pre.handCount[p]);

  if (memcmp(&pre, post, sizeof(struct gameState)) != 0)
  {
    printf("MEMCMP: %d (%d)\n", memcmp(&pre, post, sizeof(struct gameState)), handPos);
    return 5;
  }

  // all tests passed
  return 0;

}

// TEST ALL GAME STATE
/*
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
*/
