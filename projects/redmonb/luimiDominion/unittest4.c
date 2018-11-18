//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//     NAME: Benjamin Redmon
//     TEST: Unit Test 4
// FUNCTION: whoseTurn()
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <string.h>

int main()
{
  struct gameState game;
  int k[10] = {adventurer, gardens, embargo, village, minion,
               mine, cutpurse, sea_hag, tribute, smithy},
      seed = 50,
      testNum = 1;


  printf("Whose Turn\n");
  // initialize new game with i players
  for (int i = 2; i <= 4; i++)
  {
    // initialize new game with i players
    initializeGame(i, k, seed, &game);

    // check each player
    for (int j = 0; j < i; j++)
    {
      game.whoseTurn = j;

      if (whoseTurn(&game) == j)
        printf("%2d.) Tested = %d, Actual = %d: Passed\n", testNum, j, whoseTurn(&game));
      else
        printf("%2d.) Tested = %d, Actual = %d: Passed\n", testNum, j, whoseTurn(&game));

      testNum++;
    }
  }


  return 0;
}
