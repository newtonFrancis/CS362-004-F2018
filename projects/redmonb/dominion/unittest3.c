//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//     NAME: Benjamin Redmon
//     TEST: Unit Test 3
// FUNCTION: isGameOver()
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <string.h>

const char* getCardName(int);
void testResult(struct gameState*, int);
void emptyStacks(struct gameState*, int[], int, int, int);

int main()
{
  struct gameState pre,
                   post;
  int k[10] = {adventurer, gardens, embargo, village, minion,
               mine, cutpurse, sea_hag, tribute, smithy},
      seed = 50,
      players = 2;

  // initialize a new game and make a backup copy
  initializeGame(players, k, seed, &post);
  memcpy(&pre, &post, sizeof(struct gameState));


  // full provinces -- emptying supply stacks
  printf("Supply Card Stacks\n");
  memcpy(&post, &pre, sizeof(struct gameState));  // fresh copy of game
  emptyStacks(&post, k, copper, gold, 0);

  // full provinces -- emptying kingdom cards
  printf("\nVictory Card Stacks\n");
  memcpy(&post, &pre, sizeof(struct gameState));  // fresh copy of game
  emptyStacks(&post, k, 0, 9, 1);

  // full provinces -- emptying victory cards
  printf("\nVictory Card Stacks\n");
  memcpy(&post, &pre, sizeof(struct gameState));  // fresh copy of game
  emptyStacks(&post, k, curse, duchy, 0);

  // full provinces -- emptying victory cards ascending
  printf("\nAll Kingdon Card: Ascending\n");
  memcpy(&post, &pre, sizeof(struct gameState));  // fresh copy of game
  emptyStacks(&post, k, adventurer, treasure_map, 0);

  // full provinces -- emptying victory cards descending
  printf("\nAll Kingdon Card: Descending\n");
  memcpy(&post, &pre, sizeof(struct gameState));  // fresh copy of game
  emptyStacks(&post, k, treasure_map, adventurer, 0);

  // empty provinces
  printf("\nEmpty Provinces\n");
  memcpy(&post, &pre, sizeof(struct gameState));  // fresh copy of game
  emptyStacks(&post, k, province, province, 0);

  return 0;
}


//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//        NAME: Empty Stacks
// DESCRIPTION: Emptys stacks of cards by range, using either the
//              full deck or the drawn kingdom cards.  The results
//              are tested at each stage of emptying the stacks.
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void emptyStacks(struct gameState* state, int k[10], int min, int max, int drawn)
{
  int testNum = 1,
      empty = 3;

  // when testing province, only one stack needs to be emptied
  if (min == province && max == province)
    empty = 1;

  // no empty stacks -- initialized game
  printf("%2d.) %2d empty stack(s): ", testNum, 0);
  testResult(state, 0);
  testNum++;
  int count = 1;

  // range of cards goes from low to high
  if (max >= min)
  {
    // loop through range
    for (int i = min; i <= max; i++)
    {
      // use full deck
      if (drawn == 0)
        state->supplyCount[i] = 0;
      // use kindom cards selected for game
      else
        state->supplyCount[k[i]] = 0;

      // print test info
      printf("%2d.) %2d empty stack(s): ", testNum, count);
      testNum++;
      count++;

      // test should pass -- not enough empty stacks
      if (count <= empty)
        testResult(state, 0);
      // test should fail -- too many empty stacks
      else
        testResult(state, 1);
    }
  }
  // range of cards goes from high to low
  else
  {
    for (int i = min; i >= max; i--)
    {
      // use full deck
      if (drawn == 0)
        state->supplyCount[i] = 0;
      // use kindom cards selected for game
      else
        state->supplyCount[k[i]] = 0;

      // print test info
      printf("%2d.) %2d empty stack(s): ", testNum, count);
      testNum++;
      count++;

      // test should pass -- not enough empty stacks
      if (count <= empty)
        testResult(state, 0);
      // test should fail -- too many empty stacks
      else
        testResult(state, 1);
    }
  }

}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//        NAME: Test Result
// DESCRIPTION: Prints the expected result of the game and the
//              actual result of testing isGameOver().
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void testResult(struct gameState* state, int expected)
{
  // print expected result of the game
  if (expected == 0)
    printf("Game Over -  No: ");
  else
    printf("Game Over - Yes: ");

  // print result of the test
  if (expected == 1 || expected == 0)
  {
    if (isGameOver(state) == expected)
      printf("Passed\n");
    else
      printf("Fail\n");
  }
  // expected result outside of function range
  else
  {
    printf("Expected value should be 1 or 0.\n");
  }
}
