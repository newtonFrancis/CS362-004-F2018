//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//     NAME: Benjamin Redmon
//     TEST: Card Test 4
// FUNCTION: Village
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <string.h>

int otherPlayer(struct gameState, struct gameState, int);

int main()
{
  struct gameState pre,
                   post;
  int k[10] = {adventurer, gardens, embargo, village, minion,
               mine, cutpurse, sea_hag, tribute, smithy},
      seed = 50,
      testNum = 1,
      players = 2, current = 0,
      choice1 = -1, choice2 = -1, choice3 = -1, handPos = 0, bonus = 0,
      preCount = 0, postCount = 0;

  // initialize game
  initializeGame(players, k, seed, &pre);

  // place a Village card in the current players hand
  pre.hand[current][handPos] = village;

  // get a baseline count of Village cards in the hand
  for (int i = 0; i < pre.handCount[current]; i++)
  {
    if (pre.hand[current][i] == village)
      preCount++;
  }

  // make a copy of the game
  memcpy(&post, &pre, sizeof(struct gameState));


  // TEST #1 - Village card played and discarded
  cardEffect(village, choice1, choice2, choice3, &post, handPos, &bonus);

  // get current count of Village cards after playing the card
  for (int i = 0; i < post.handCount[current]; i++)
  {
    if (post.hand[current][i] == village)
      postCount++;
  }

  // print results of the test
  printf("Village: Discarded Played Card & Net Gain +0 Cards\n");
  printf("%2d.) Village Count = %2d, Expected = %2d: ", testNum, postCount, preCount - 1);
  if (postCount == preCount - 1)
    printf("Passed\n");
  else
    printf("Fail\n");

  testNum++;

  // TEST #2 - +0 Cards
  // print results of the test comparing current and expected hand counts
  printf("%2d.)    Hand Count = %2d, Expected = %2d: ", testNum, post.handCount[current], pre.handCount[current]);
  if (post.handCount[current] == pre.handCount[current])
    printf("Passed\n");
  else
    printf("Fail\n");

  testNum++;

  // TEST #3 - +1 Action
  // print results of the test comparing current and expected hand counts
  printf("%2d.)        Action = %2d, Expected = %2d: ", testNum, post.numActions, pre.numActions + 2);
  if (post.numActions == pre.numActions + 2)
    printf("Passed\n");
  else
    printf("Fail\n");

  testNum++;

  printf("%2d.)  Unchanged Game State of Player 2: ", testNum);
  otherPlayer(pre, post, 1);

  return 0;
}

int otherPlayer(struct gameState pre, struct gameState post, int player)
{
  // compare pre and post hand of the other player
  if (pre.handCount[player] == post.handCount[player])
  {
    for (int i = 0; i < pre.handCount[player]; i++)
    {
      if (pre.hand[player][i] != post.hand[player][i])
      {
        printf("Fail\n");
        return 1;
      }
    }
  }
  else
  {
    printf("Fail\n");
    return 1;
  }

  // compare pre and post deck of the other player
  if (pre.deckCount[player] == post.deckCount[player])
  {
    for (int i = 0; i < pre.deckCount[player]; i++)
    {
      if (pre.deck[player][i] != post.deck[player][i])
      {
        printf("Fail\n");
        return 1;
      }
    }
  }
  else
  {
    printf("Fail\n");
    return 1;
  }

  // compare pre and post discard pile of the other player
  if (pre.discardCount[player] == post.discardCount[player])
  {
    for (int i = 0; i < pre.discardCount[player]; i++)
    {
      if (pre.discard[player][i] != post.discard[player][i])
      {
        printf("Fail\n");
        return 1;
      }
    }
  }
  else
  {
    printf("Fail\n");
    return 1;
  }

  printf("Passed\n");
  return 1;
}
