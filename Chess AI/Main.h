#ifndef __Main_h
#define __Main_h

#include <stdint.h>
#include "MACROS.h"
#include "Game.h"
#include "Utils.h"

const int TYPE_VALUE[7] = {
    0,   //None
    10,  //Pawn
    30,  //Knight
    30,  //Bishop
    50,  //Rook
    80,  //Queen
    0    //King
};

const int TYPE_VALUE_FEW[7] =
{
        0,
        10,
        29,
        31,
        50,
        80,
        0
};

//Guess what, makes #depth random moves
void makeRandomMoves(GameState *state, int depth);

//Heuristic minimax
int Hminimax(GameState *state, int depth);

//Iterative deepening (helps with move ordering also)
Move *IDS(GameState *state);
//Almost the same as alpha-beta search, it just does not free the moves given to it
Move *noFreeABS(GameState *state, LegalMoves* actions, int depth);

//Alpha-Beta-Search with corresponding function maxval and minval
Move *ABS(GameState *state, LegalMoves *lm, int depth);
int maxval(GameState *state, int a, int b, int depth);
int minval(GameState *state, int a, int b, int depth);

int eval(GameState *state, LegalMoves *lm);


#endif
