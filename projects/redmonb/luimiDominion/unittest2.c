//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//     NAME: Benjamin Redmon
//     TEST: Unit Test 2
// FUNCTION: numHandCards()
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <string.h>

int main()
{
  struct gameState pre,
                   post;
  int k[10] = {adventurer, gardens, embargo, village, minion,
               mine, cutpurse, sea_hag, tribute, smithy},
      seed = 50,
      players = 2,
      testNum = 1;

  // initialize a new game and make a backup copy
  initializeGame(players, k, seed, &post);
  memcpy(&pre, &post, sizeof(struct gameState));

  // test five hands alternating players
  printf("Number of Cards in Hand\n");
  for (int i = 0; i < 5; i++)
  {
    for (int j = 0; j < players; j++)
    {
      post.whoseTurn = j;
      drawCard(j, &post);

      if (post.handCount[j] == numHandCards(&post))
      {
        printf("%2d.) Cards = %2d, Expected = %2d : Passed\n", testNum, post.handCount[j], numHandCards(&post));
      }
      else
      {
        printf("%2d.) Cards = %2d, Expected = %2d : Fail\n", testNum, post.handCount[j], numHandCards(&post));
      }

      testNum++;
    }
  }

  return 0;
}
