//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//     NAME: Benjamin Redmon
//     TEST: Unit Test 1
// FUNCTION: initializeGame()
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <string.h>

int main()
{
  struct gameState game;
  int k1[10] = {adventurer, gardens, embargo, village, minion,
                mine, cutpurse, sea_hag, tribute, smithy},
      k2[10] = {adventurer, gardens, embargo, village, minion,
                mine, cutpurse, sea_hag, smithy, smithy},
      seed = 50,
      players = 2,
      testNum = 1;

  // test number of players at, below, and above allowed range
  printf("Correct Number of Players\n");
  for (int i = 1; i <= 5; i++)
  {
    if (i < 2 || i > 4)
    {
      if (initializeGame(i, k1, seed, &game) == -1)
        printf("%2d.) %2d Players - Incorrect: Passed\n", testNum, i);
      else
        printf("%2d.) %2d Players - Correct:   Fail\n", testNum, i);
    }
    else
    {
      if (initializeGame(i, k1, seed, &game) == -1)
        printf("%2d.) %2d Players - Correct:   Fail\n", testNum, i);
      else
        printf("%2d.) %2d Players - Correct:   Passed\n", testNum, i);
    }

    testNum++;
  }


  printf("\nDuplicate Kingdom Cards\n");
  // working set of kingdom cards
  if (initializeGame(players, k1, seed, &game) == -1)
    printf("%2d.) Correct Set:   Fail\n", testNum);
  else
    printf("%2d.) Correct Set:   Passed\n", testNum);

  testNum++;

  // incorrect set of kingdom cards with duplicates
  if (initializeGame(players, k2, seed, &game) == -1)
    printf("%2d.) Duplicate Set: Passed\n", testNum);
  else
    printf("%2d.) Duplicate Set: Fail\n", testNum);


  printf("\nSupply Allocation\n");
  for (int i = 2; i <= 4; i++)
  {
    int result = 0;

    // initialize new game with i players
    initializeGame(i, k1, seed, &game);

    // two players
    if (game.numPlayers == 2)
    {
      if (game.supplyCount[curse] != 10)
        result = 1;

      if (game.supplyCount[estate] != 8 || game.supplyCount[duchy] != 8 || game.supplyCount[province] != 8)
        result = 2;

    }
    // three or four players
    else
    {
      if (game.supplyCount[estate] != 12 || game.supplyCount[duchy] != 12 || game.supplyCount[province] != 12)
        result = 3;

      // three players
      if (game.numPlayers == 3)
      {
        if (game.supplyCount[curse] != 20)
          result = 4;
      }
      // four players
      else
      {
        if (game.supplyCount[curse] != 30)
          result = 5;
      }
    }

    // check supply of copper, silver, and gold
    if (game.supplyCount[copper] != 60 - (7 * i))
      result = 6;

    if (game.supplyCount[silver] != 40 || game.supplyCount[gold] != 30)
      result = 7;

    // kingdom card allocation
    for (int j = 0; j < 10; j++)
    {
      if (k1[j] == great_hall || k1[j] == gardens)
      {
        if (game.numPlayers == 2 && game.supplyCount[k1[j]] != 8)
          result = 8;
        else if (game.numPlayers > 2 && game.supplyCount[k1[j]] != 12)
          result = 9;
      }
      else
      {
        if (game.supplyCount[k1[i]] != 10)
          result = 10;
      }


    }

    if (result != 0)
      printf("%2d.) %d Players - Error #%2d: Fail\n", testNum, i, result);
    else
      printf("%2d.) %d Players - Allocating Supplies: Passed\n", testNum, i);

    testNum++;
  }

  printf("\nInitialize Game Play\n");
  // initialize new game with i players
  for (int i = 2; i <= 4; i++)
  {
    int result = 0;

    // initialize new game with i players
    initializeGame(i, k1, seed, &game);

    // check each player
    for (int j = 0; j < i; j++)
    {
      if (game.whoseTurn == j)
      {
        if (game.outpostPlayed != 0)
          result = 1;
        if (game.phase != 0)
          result = 2;
        if (game.numActions != 1)
          result = 3;
        if (game.numBuys != 1)
          result = 4;
        if (game.playedCardCount != 0)
          result = 5;
        if (game.handCount[j] != 5)
          result = 6;
      }
      else
      {
        if (game.handCount[j] != 0 || game.discardCount[j] !=0)
          result = 7;
      }

      if (result != 0)
        printf("%2d.) %d Players - Error #%2d: Fail\n", testNum, i, result);
      else
        printf("%2d.) %d Players - initialize Game: Passed\n", testNum, i);

      testNum++;
    }
  }

  return 0;
}
