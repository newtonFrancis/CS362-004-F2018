//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// NAME: Benjamin Redmon
// TEST: Random Card Test #2
// CARD: Steward
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

int checkSteward(struct gameState*, int, int, int*, int, int, int);

int main()
{
  struct gameState G;
  int p,
      h,
      bonus = -1,
      test,
      choice1, choice2, choice3,
      pass = 0, fail = 0, code = 0;

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
    G.playedCardCount = 0;      // no cards played

    // set deck and discard count to max deck size
    G.deckCount[p] = floor(Random() * MAX_DECK);
    G.discardCount[p] = floor(Random() * MAX_DECK);

    // set hand count to max hand size
    G.handCount[p] = floor(Random() * MAX_HAND);

    // set hand count to at least three -- to hold the steward card
    if (G.handCount[p] < 3)
      G.handCount[p] = 3;

    // randomly select a hand position and set to steward
    h = floor(Random() * G.handCount[p]);
    G.hand[p][h] = steward;

    choice1 = floor(Random() * 3) + 1;

    choice2 = h;
    choice3 = h;

    while (choice2 == h || choice3 == h || choice2 == choice3)
    {
      choice2 = floor(Random() * G.handCount[p]);
      choice3 = floor(Random() * G.handCount[p]);
    }

    // run the checkSteward test -- returns greater than 0 if failed
    test = checkSteward(&G, p, h, &bonus, choice1, choice2, choice3);

    // check if steward test failed
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

int checkSteward(struct gameState *post, int p, int handPos, int* bonus, int choice1, int choice2, int choice3)
{
  struct gameState pre;

  // create a copy of the current game state
  memcpy(&pre, post, sizeof(struct gameState));

  // run card effect for the steward card
  cardEffect(post->hand[post->whoseTurn][handPos], choice1, choice2, choice3, post, handPos, bonus);

  // card option #1 - draw two cards
  if (choice1 == 1)
  {
    // increase hand two and decrease deck two
    pre.handCount[p] = pre.handCount[p] + 2;

    // enough cards in deck to draw three
    if (pre.deckCount[p] >= 2)
    {
      // subtract the drawn cards
      pre.deckCount[p] = pre.deckCount[p] - 2;
    }
    // not enough cards in deck to draw three
    else if (pre.deckCount[p] < 2)
    {
      // account for re-shuffled discarded deck when empty
      pre.deckCount[p] = pre.discardCount[p] - 2 + pre.deckCount[p];

      // copy discard to reflect cards discarded during play
      memcpy(pre.discard[p], post->discard[p], sizeof(int) * pre.discardCount[p]);

      pre.discardCount[p] = 0;
    }

    // remove steward from hand
    pre.hand[p][handPos] = pre.hand[p][pre.handCount[p] - 1];
    pre.hand[p][pre.handCount[p] - 1] = -1;
    pre.handCount[p]--;

    // copy hand to reflect cards drawn during play
    memcpy(pre.hand[p], post->hand[p], sizeof(int) * pre.handCount[p]);
    memcpy(pre.deck[p], post->deck[p], sizeof(int) * (pre.deckCount[p] + 2));
  }
  else
  {
    // card option #2 - add two coins
    if (choice1 == 2)
    {
      // coins increase by two
      pre.coins = pre.coins + 2;

    }
    // card option #3 - discard two cards
    else
    {
      pre.hand[p][choice2] = pre.hand[p][pre.handCount[p] - 1];
      pre.hand[p][pre.handCount[p] - 1] = -1;
      pre.handCount[p]--;

      pre.hand[p][choice3] = pre.hand[p][pre.handCount[p] - 1];
      pre.hand[p][pre.handCount[p] - 1] = -1;
      pre.handCount[p]--;
    }

    // remove steward from hand
    pre.hand[p][handPos] = pre.hand[p][pre.handCount[p] - 1];
    pre.hand[p][pre.handCount[p] - 1] = -1;
    pre.handCount[p]--;
  }

  if (handPos >= pre.handCount[p] + 1)
    pre.playedCards[pre.playedCardCount] = -1;
  else
    pre.playedCards[pre.playedCardCount] = steward;

  // set played card to steward
  //pre.playedCards[pre.playedCardCount] = steward;
  pre.playedCardCount++;


  if (pre.coins != post->coins)
  {
    // failed coin increase
    return 1;
  }

  if (pre.handCount[p] != post->handCount[p])
  {
    // failed hand count
    printf("handCount: %d vs. %d  (%d/%d/%d/%d)\n", pre.handCount[p], post->handCount[p], choice1, handPos, choice2, choice3);
    return 2;
  }

  // played card count incremented
  if (pre.playedCardCount != post->playedCardCount)
  {
    // failed played card count
    return 3;
  }

  // played card was a steward
  if (pre.playedCards[pre.playedCardCount - 1] != post->playedCards[post->playedCardCount - 1])
  {
    // failed played steward card
    printf("playedCards[%d]: %d vs. %d  (%d/%d/%d/%d)\n", pre.playedCardCount - 1, pre.playedCards[pre.playedCardCount - 1], post->playedCards[pre.playedCardCount - 1], choice1, handPos, choice2, choice3);
    return 4;
  }

  // compare all memory in the gameState
  if (memcmp(&pre, post, sizeof(struct gameState)) != 0)
  {
    printf("MEMCMP: %d (%d/%d/%d/%d)\n", memcmp(&pre, post, sizeof(struct gameState)), choice1, handPos, choice2, choice3);
    return 5;
  }

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
        printf("hand[%d][%d]: %d vs. %d  (%d/%d/%d/%d)\n", i, j, pre.hand[i][j], post->hand[i][j], choice1, handPos, choice2, choice3);
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
// END TEST
