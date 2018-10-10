#ifndef _DOMINION_HELPERS_H
#define _DOMINION_HELPERS_H

#include "dominion.h"

int drawCard(int player, struct gameState *state);
int updateCoins(int player, struct gameState *state, int bonus);
int discardCard(int handPos, int currentPlayer, struct gameState *state,
		int trashFlag);
int gainCard(int supplyPos, struct gameState *state, int toFlag, int player);
int getCost(int cardNumber);
int cardEffect(int card, int choice1, int choice2, int choice3,
	       struct gameState *state, int handPos, int *bonus);

 // refactored cardEffect Cards
 void refAdventurer(struct gameState*, int, int, int*, int, int);
 void refSmithy(struct gameState*, int, int);
 void refGreatHall(struct gameState*, int, int);
 void refSeaHag(struct gameState*, int);
 void refCouncilRoom(struct gameState*, int, int);

#endif
