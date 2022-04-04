#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "Main.h"
static int nodesEvaluated = 0; //Used for testing

const int MIN_INTEGER = -2147483648;
const int MAX_INTEGER = 2147483647;

clock_t t;
int timeExceeded = 0;


int main()
{
    LegalMoves *lm;
    GameState *state = newGame();
    Move *bestMove, *performMove;
    int input[4];

    printf("\nTo make a move type in the following 4 ints: fromX fromY toX toY\n");
    printf("Example: '0' '1' '0' '3' will move the piece at (0,1) to (0,3)\n");
    printf("\nTo promote to a knight type '0', to promote to a queen type '1', there will be reminders for this when the option is available\n");


    sleep(3);

    while(1)
    {
        //check if black won
        lm = legalMoves(state);
        if(lm->count == 0) {
            printf("BLACK WON!\n");
            exit(0);
        }
        for(int i = 0; i < lm->count; ++i)
            free(lm->moves[i]);
        free(lm);

        printf("White is thinking...\n");
        timeExceeded = 0;
        nodesEvaluated = 0; //reset node evaluation counter
        t = clock();
        bestMove = IDS(state);
        applyMove(state,bestMove);
        printBoard(state);

        lm = legalMoves(state);
        if(lm->count == 0)
        {
            printf("WHITE WON\n");
            exit(0);
        }
        printf("What move do you want to perform?\n");
        printf("Give input: 'fromX' 'fromY' 'toX' 'toY'\n");
        scanf("%d %d %d %d",&input[0],&input[1],&input[2],&input[3]);
        for(int i = 0; i < lm->count; ++i)
        {
            if(input[0] == lm->moves[i]->fromX &&
            input[1] == lm->moves[i]->fromY &&
            input[2] == lm->moves[i]->toX &&
            input[3] == lm->moves[i]->toY)
            {
                performMove = lm->moves[i];
                if(lm->moves[i]->promotion == KNIGHT || lm->moves[i]->promotion == QUEEN) {
                    printf("Type '0' to promote to a queen and '1' to promote to a knight\n");
                    scanf("%d", &input[0]);
                    if (input[0] == 1)
                        performMove = lm->moves[i + 1];
                }
            }
        }
        applyMove(state,performMove);
        printBoard(state);
        for(int i = 0; i < lm->count; ++i)
            if(lm->moves[i] != performMove)
                free(lm->moves[i]);
        free(lm);
    }
}

Move *IDS(GameState *state)
{
    LegalMoves *lm = legalMoves(state);
    Move *bestMove = lm->moves[0];
    for(int i = 1; !timeExceeded; ++i)
    {
        bestMove = noFreeABS(state,lm,i);       //Get a new best move from one ply deeper
        int bestMoveFirst = 0;                  //Start by putting the best move to the front of the list
        for(int j = 0; !bestMoveFirst; ++j) {   //Put best move in front of list (move ordering), so the next call to ABS has the best move in front
            if(lm->moves[j] == bestMove) {
                lm->moves[j] = lm->moves[0];
                lm->moves[0] = bestMove;
                bestMoveFirst = 1;
            }
        }
    }
    //Clean up legalMoves and the moves therein, except for bestMove
    for(int i = 0; i<lm->count; ++i)
        if(lm->moves[i] != bestMove)
            free(lm->moves[i]);
    free(lm);
    return bestMove;
}

/* Description:
 * Try to apply each possible move, and for each move
 * check the enemy's responses. Return the move for which
 * the enemy has the worst response. Also, this version does
 * not free the moves given to it.
 */
Move *noFreeABS(GameState *state, LegalMoves* actions, int depth)
{
    int a = MIN_INTEGER;
    Move *bestMove;
    for(int i = 0; i < actions->count; ++i)
    {
        applyMove(state, actions->moves[i]);
        int minResponse = minval(state,a,MAX_INTEGER,depth-1);
        if(a < minResponse) {
            a = minResponse;
            bestMove = actions->moves[i];
        }
        undoMove(state);
    }
    return bestMove;
}

/* Description:
 * Try to apply each possible move, and for each move
 * check the enemy's responses. Return the move for which
 * the enemy has the worst response.
 */
Move *ABS(GameState *state, LegalMoves* actions, int depth)
{
    int a = MIN_INTEGER;
    Move *bestMove = NULL;
    for(int i = 0; i < actions->count; ++i)
    {
        applyMove(state, actions->moves[i]);
        int minResponse = minval(state,a,MAX_INTEGER,depth-1);
        if(a < minResponse) {
            a = minResponse;
            free(bestMove);
            bestMove = actions->moves[i];
            undoMove(state);
        }
        else
        {
            undoMove(state);
            free(actions->moves[i]);
        }
    }
    return bestMove;
}

int maxval(GameState *state, int a, int b, int depth)
{
    LegalMoves *lm = legalMoves(state);     //We need legalmoves to determine if the game is over
    if(depth == 0 || lm->count == 0) {      //Cutoff test
        int evaluation = eval(state, lm);   //Evaluate state
        for (int i = 0; i < lm->count; ++i) //Remember to free malloc'ed memory
            free(lm->moves[i]);
        free(lm);
        return evaluation;
    }
    int v = MIN_INTEGER;
    for(int i = 0; i < lm->count; ++i)
    {
        applyMove(state,lm->moves[i]);            //Apply move
        int evaluation = minval(state,a,b,depth-1); //Evaluate new state
        undoMove(state);
        free(lm->moves[i]);
        if(v < evaluation)
            v = evaluation;
        if(v >= b)
            return v;
        if(a > v)
            a = v;
    }
    free(lm);
    return v;
}

int minval(GameState *state, int a, int b, int depth)
{
    LegalMoves *lm = legalMoves(state);     //We need legalmoves to determine if the game is over
    if(depth == 0 || lm->count == 0) {      //Cutoff test
        int evaluation = eval(state, lm);   //Evaluate state
        for (int i = 0; i < lm->count; ++i) //Remember to free malloc'ed memory
            free(lm->moves[i]);
        free(lm);
        return evaluation;
    }
    int v = MAX_INTEGER;
    for(int i = 0; i < lm->count; ++i)
    {
        applyMove(state,lm->moves[i]);            //Apply move
        int evaluation = maxval(state,a,b,depth-1); //Evaluate new state
        undoMove(state);
        free(lm->moves[i]);
        if(v > evaluation)
            v = evaluation;
        if(v <= a)
            return v;
        if(b < v)
            b = v;
    }
    free(lm);
    return v;
}

int Hminimax(GameState *state, int depth)
{

    int maxOrMin;
    LegalMoves *lm = legalMoves(state);

    if(depth == 0 || lm->count == 0) {      //Cutoff test
        maxOrMin = eval(state, lm);
        for (int i = 0; i < lm->count; ++i)
            free(lm->moves[i]);
    } else if(state->turn == WHITE) {        //MAX
        maxOrMin = MIN_INTEGER;
        for(int i = 0; i < lm->count; ++i) {
            applyMove(state, lm->moves[i]);
            int evalValue = Hminimax(state, depth - 1);
            maxOrMin = maxOrMin >= evalValue ? maxOrMin : evalValue;
            undoMove(state);
            free(lm->moves[i]);
        }
    } else { //Min
        maxOrMin = MAX_INTEGER;
        for(int i = 0; i < lm->count; ++i) {
            applyMove(state, lm->moves[i]);
            int evalValue = Hminimax(state, depth - 1);
            maxOrMin = maxOrMin <= evalValue ? maxOrMin : evalValue;
            undoMove(state);
            free(lm->moves[i]);
        }
    }
    free(lm);
    return maxOrMin;
}

int eval(GameState *state, LegalMoves *lm)
{
    ++nodesEvaluated;
    if((nodesEvaluated&0xffff) == 0)    //Every 2^16 nodes it checks the time
    {
        double timeSpent = ((double)clock()-t)/CLOCKS_PER_SEC;
        if(timeSpent > 1.3)
            timeExceeded = 1;
    }
    if(lm->count == 0)          //Game over
        return utility(state);

    int res = 0;
    if(state->liveWhite + state->liveBlack <= 12) //Few pieces left, bishops worth more and knights less
    {
        for(int i = 1; i < state->liveWhite; ++i)
            res += TYPE_VALUE_FEW[state->whitePieces[i]->type]; //Add value of white pieces
        for(int i = 1; i < state->liveBlack; ++i)
            res -= TYPE_VALUE_FEW[state->blackPieces[i]->type]; //Subtract value of black pieces
    }
    else
    {
        for(int i = 1; i < state->liveWhite; ++i)
            res += TYPE_VALUE[state->whitePieces[i]->type]; //Add value of white pieces
        for(int i = 1; i < state->liveBlack; ++i)
            res -= TYPE_VALUE[state->blackPieces[i]->type]; //Subtract value of black pieces
    }
    return res;
}


//Used Generating Random States
void makeRandomMoves(GameState *state, int depth)
{
    srand(time(NULL));
    for(int i = 0; i < depth; ++i)
    {
        LegalMoves *lm = legalMoves(state);
        if(lm->count == 0)
        {
            free(lm);
            return;
        }

        int random = rand() % lm->count;
        applyMove(state,lm->moves[random]);

        for(int j = 0; j < lm->count; ++j) //Free other moves, but keep random move in memory
            if(j != random)
                free(lm->moves[j]);
        free(lm);
    }
}



