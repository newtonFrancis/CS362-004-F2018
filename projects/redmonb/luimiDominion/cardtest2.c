//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//     NAME: Benjamin Redmon
//     TEST: Card Test 3
// FUNCTION: Adventurer
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

  // place a Adventurer card in the current players hand
  pre.hand[current][handPos] = adventurer;

  // get a baseline count of Adventurer cards in the hand
  for (int i = 0; i < pre.handCount[current]; i++)
  {
    if (pre.hand[current][i] == copper || pre.hand[current][i] == silver || pre.hand[current][i] == gold)
      preCount++;
  }

  // make a copy of the game
  memcpy(&post, &pre, sizeof(struct gameState));


  // TEST #1 - Adventurer card played and discarded
  cardEffect(adventurer, choice1, choice2, choice3, &post, handPos, &bonus);

  // get current count of Adventurer cards after playing the card
  for (int i = 0; i < post.handCount[current]; i++)
  {
    if (post.hand[current][i] == copper || post.hand[current][i] == silver || post.hand[current][i] == gold)
      postCount++;
  }

  // print results of the test
  printf("Adventurer: Draw Until Adding Two Additional Treasure\n");
  printf("%2d.) Treasure Count = %2d, Expected = %2d: ", testNum, postCount, preCount + 2);
  if (postCount == preCount + 2)
    printf("Passed\n");
  else
    printf("Fail\n");

  testNum++;

  // TEST #2 - +0 Cards
  // print results of the test comparing current and expected hand counts
  printf("%2d.)     Hand Count = %2d, Expected = %2d: ", testNum, post.handCount[current], pre.handCount[current] + 2);
  if (post.handCount[current] == pre.handCount[current] + 2)
    printf("Passed\n");
  else
    printf("Fail\n");

  testNum++;

  printf("%2d.)   Unchanged Game State of Player 2: ", testNum);
  otherPlayer(pre, post, 1);

  return 0;
}

// compare the pre and post game state of player 2
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
