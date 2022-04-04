#include <stdlib.h>
#include "Utils.h"

const uint8 diagonals[4][2] = {{1,1},{1,-1},{-1,-1},{-1,1}};
const uint8 nonDiagonals[4][2] = {{0,1},{1,0},{0,-1},{-1,0}};
const uint8 allDirections[8][2] = {{1,1},{1,-1},{-1,-1},{-1,1},{0,1},{1,0},{0,-1},{-1,0}};
const uint8 horseJumps[8][2] = {{1,2},{2,1},{2,-1},{1,-2},{-1,-2},{-2,-1},{-2,1},{-1,2}};
const uint8 kingMoves[8][2] = {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};
void (*getMoves[7])(GameState*, Piece*, LegalMoves*) = {NULL,getPawnMoves,getKnightMoves,getBishopMoves,getRookMoves,getQueenMoves,NULL}; //Function pointers

Piece *NONEPIECE = &(Piece){NONE, NOCOLOR, 0, 0, 0};

GameState *newGame()
{
    MALLOC(GameState,state);
    state->moveCounter = 0;
    for(int i = 0; i <1000; ++i)
        state->movesMade[i] = NULL;
    state->liveWhite = 16;
    state->liveBlack = 16;
    state->turn = WHITE;
    state->enPassant = NONEPIECE;

    for(int y = 2; y < 7; ++y)
        for(int x = 0; x < 8; ++x)
            state->board[x][y] = NONEPIECE;
    for(int x = 0; x < 8; ++x) {
        Piece *whitePawn = makePiece(PAWN, WHITE, x, 1);
        Piece *blackPawn = makePiece(PAWN, BLACK, x, 6);
        state->board[x][1] = state->whitePieces[x+8] = whitePawn;
        state->board[x][6] = state->blackPieces[x+8] = blackPawn;
    }

    for(int x = 0; x < 8; ++x) {
        if(x==0|| x==7) {
            state->board[x][0] = state->whitePieces[x] = makePieceWithInfo(ROOK, WHITE, x, 0, 1);
            state->board[x][7] = state->blackPieces[x] = makePieceWithInfo(ROOK, BLACK, x, 7, 1);
        } else if(x==1||x==6) {
            state->board[x][0] = state->whitePieces[x] = makePiece(KNIGHT, WHITE, x, 0);
            state->board[x][7] = state->blackPieces[x] = makePiece(KNIGHT, BLACK, x, 7);
        } else if(x==2||x==5) {
            state->board[x][0] = state->whitePieces[x] = makePiece(BISHOP, WHITE, x, 0);
            state->board[x][7] = state->blackPieces[x] = makePiece(BISHOP, BLACK, x, 7);
        } else if(x==3) {
            state->board[x][0] = state->whitePieces[x] = makePiece(QUEEN, WHITE, x, 0);
            state->board[x][7] = state->blackPieces[x] = makePiece(QUEEN, BLACK, x, 7);
        } else {
            state->board[x][0] = state->whitePieces[x] = makePieceWithInfo(KING, WHITE, x, 0, 1);
            state->board[x][7] = state->blackPieces[x] = makePieceWithInfo(KING, BLACK, x, 7, 1);
        }
    }

    //Move king to front of both lists
    Piece *aux;
    aux = state->whitePieces[0];
    state->whitePieces[0] = state->whitePieces[4];
    state->whitePieces[4] = aux;

    aux = state->blackPieces[0];
    state->blackPieces[0] = state->blackPieces[4];
    state->blackPieces[4] = aux;
    return state;
}

LegalMoves *legalMoves(GameState *state)
{
    MALLOC(LegalMoves, lm);
    lm->count = 0;

    Piece **pieces = state->turn == WHITE ? state->whitePieces : state->blackPieces;
    uint8 *livePieces = state->turn == WHITE ? &state->liveWhite : &state->liveBlack;

    Piece *kingAttackers[2] = {NULL,NULL};
    getKingAttackers(state, kingAttackers);

    getKingMoves(state, pieces[0], lm, kingAttackers);
    uint8 kingMoveCount = lm->count;

    if(kingAttackers[1] == NULL) //Not 2 attackers
        for(int i = 1; i < *livePieces; ++i)
            (*getMoves[pieces[i]->type])(state,pieces[i],lm);
    Piece *king = state->turn == WHITE ? state->whitePieces[0] : state->blackPieces[0];

    //At this point the non-king moves are not entirely legal, we need to check if the king is left in check after the move
    for(int i = kingMoveCount; i < lm->count; ++i)
    {
        uint8 fromX = lm->moves[i]->fromX;
        uint8 fromY = lm->moves[i]->fromY;
        uint8 toX = lm->moves[i]->toX;
        uint8 toY = lm->moves[i]->toY;
        uint8 deadX = lm->moves[i]->dead->x;
        uint8 deadY = lm->moves[i]->dead->y;
        Piece *mover = state->board[fromX][fromY];

        //Move pieces
        state->board[toX][toY] = mover;
        state->board[fromX][fromY] = NONEPIECE;
        if(lm->moves[i]->promotion == KING)
            state->board[deadX][deadY] = NONEPIECE;

        //If king was in check, remove this move from the list
        int illegal = tileAttacked(state,king->x,king->y);

        //Move pieces back into place
        state->board[fromX][fromY] = mover;
        state->board[toX][toY] = NONEPIECE;
        if(lm->moves[i]->dead != NONEPIECE)
            state->board[deadX][deadY] = lm->moves[i]->dead;

        //Weed out the illegal move
        if(illegal) {
            free(lm->moves[i]);
            lm->moves[i--] = lm->moves[--lm->count];
        }
    }
    return lm;
}

inline uint8 onBoard(uint8 x, uint8 y)
{
    return 0 <= x && x < 8 && 0 <= y && y < 8;
}

inline uint8 onBoard2(uint8 x)
{
    return 0 <= x && x < 8;
}

inline Piece *rayCast(GameState *state, uint8 x, uint8 y, uint8 stepX, uint8 stepY)
{
    x+=stepX;
    y+=stepY;
    while(onBoard(x,y)) {
        if(state->board[x][y] != NONEPIECE)
            return state->board[x][y];
        x+=stepX;
        y+=stepY;
    }
    return NONEPIECE;
}

void getKingAttackers(GameState *state, Piece **attackersList)
{
    Piece *king = state->turn ? state->blackPieces[0] : state->whitePieces[0];
    uint8 attackers = 0;
    uint8 x = king->x;
    uint8 y = king->y;

    for(int i = 0; i < 8; ++i) { //Check if knight attacks king
        uint8 knightX = x+horseJumps[i][0];
        uint8 knightY = y+horseJumps[i][1];
        if(onBoard(knightX,knightY) && state->board[knightX][knightY]->type == KNIGHT && state->board[knightX][knightY]->color != king->color)
            attackersList[attackers++] = state->board[knightX][knightY];
    }
    for(int i = 0; i < 4; ++i) { //Check if diagonal moving pieces attack king
        Piece *aux = rayCast(state,x,y,diagonals[i][0],diagonals[i][1]);
        if(aux->color != king->color &&
        (aux->type == BISHOP || aux->type == QUEEN ||
        (aux->type == PAWN && aux->y == y+1 - king->color*2))) //Yes, horrible boolean expression
            attackersList[attackers++] = aux;
    }
    for(int i = 0; i < 4; ++i) //Check if nonDiagonal moving pieces attack king
    {
        Piece *aux = rayCast(state,x,y,nonDiagonals[i][0],nonDiagonals[i][1]);
        if(aux->color != king->color && (aux->type == ROOK || aux->type == QUEEN))
            attackersList[attackers++] = aux;
    }
}

uint8 tileAttacked(GameState *state, uint8 x, uint8 y)
{
    for(int i = 0; i < 8; ++i) { //Check if knight attack tile
        uint8 knightX = x+horseJumps[i][0];
        uint8 knightY = y+horseJumps[i][1];
        if(onBoard(knightX,knightY) && state->board[knightX][knightY]->type == KNIGHT &&
        state->board[knightX][knightY]->color != state->turn)
            return 1;
    }
    for(int i = 0; i < 4; ++i) { //Check if diagonal moving pieces attack tile
        Piece *aux = rayCast(state,x,y,diagonals[i][0],diagonals[i][1]);
        if(aux->color != state->turn &&
        (aux->type == BISHOP || aux->type == QUEEN ||
        (aux->type == PAWN && aux->y == y + 1 - state->turn*2))) //Yes, horrible boolean expression
            return 1;
    }
    for(int i = 0; i < 4; ++i) { //Check if nonDiagonal moving pieces attack tile
        Piece *aux = rayCast(state,x,y,nonDiagonals[i][0],nonDiagonals[i][1]);
        if(aux->color != state->turn && (aux->type == ROOK || aux->type == QUEEN))
            return 1;
    }
    for(int i = 0; i < 8; ++i) //Check if enemy king attacks it
    {
        uint8 kingX = x+allDirections[i][0];
        uint8 kingY = y+allDirections[i][1];

        if(onBoard(kingX,kingY) &&
        state->board[kingX][kingY]->type == KING &&
        state->board[kingX][kingY]->color != state->turn)
            return 1;
    }
    return 0;
}

inline void addLegalMove(LegalMoves* legalMoves, Move* move)
{
    legalMoves->moves[legalMoves->count++] = move;
}

void getKingMoves(GameState *state, Piece *king, LegalMoves *legalMoves, Piece **kingAttackers)
{
    uint8 x = king->x;
    uint8 y = king->y;
    Type promotion = NONE;
    if(king->info == 1)
        promotion = BISHOP;
    for(int i = 0; i < 8; ++i)
    {
        uint8 newX = x + kingMoves[i][0];
        uint8 newY = y + kingMoves[i][1];
        if(onBoard(newX,newY) && !tileAttacked(state,newX,newY) && state->board[newX][newY]->color != king->color)
            addLegalMove(legalMoves,makeMove(x,y,newX,newY,state->board[newX][newY], promotion));
    }

    //Check if castling is an option
    if(king->info == 1 && kingAttackers[0] == NULL)
    {
        //Check long castle
        if(state->board[0][y]->info == 1 &&
           state->board[1][y] == NONEPIECE &&
           state->board[2][y] == NONEPIECE && !tileAttacked(state,2,y) &&
           state->board[3][y] == NONEPIECE && !tileAttacked(state,3,y))
            addLegalMove(legalMoves,makeMove(x,y,x-2,y,NONEPIECE, promotion)); //Promotion to Bishop means castle happened
        //Check short castle
        if(state->board[7][y]->info == 1 &&
           state->board[6][y] == NONEPIECE && !tileAttacked(state,6,y) &&
           state->board[5][y] == NONEPIECE && !tileAttacked(state,5,y))
            addLegalMove(legalMoves,makeMove(x,y,x+2,y,NONEPIECE, promotion)); //Promotion to Bishop means castle happened
    }
}

void getPawnMoves(GameState *state, Piece *pawn, LegalMoves *legalMoves)
{
    int direction = 1 - (int)(state->turn*2);
    uint8 x = pawn->x;
    uint8 y = pawn->y;
    uint8 newY = y + direction;
    uint8 promote = newY == 0 || newY == 7;

    if(onBoard2(x) && state->board[x][newY] == NONEPIECE) //Normal step forward
    {
        if(promote)
        {
            addLegalMove(legalMoves,makeMove(x,y,x,newY,NONEPIECE,QUEEN));
            addLegalMove(legalMoves, makeMove(x,y,x,newY,NONEPIECE,KNIGHT));
        }
        else
            addLegalMove(legalMoves,makeMove(x,y,x,newY,NONEPIECE,NONE));
    }
    if(onBoard2(x-1) && state->board[x-1][newY] != NONEPIECE && state->board[x-1][newY]->color != pawn->color) //Capture diagonally
    {
        if(promote)
        {
            addLegalMove(legalMoves,makeMove(x,y,x-1,newY,state->board[x-1][newY],QUEEN));
            addLegalMove(legalMoves,makeMove(x,y,x-1,newY,state->board[x-1][newY],KNIGHT));
        }
        else
            addLegalMove(legalMoves,makeMove(x,y,x-1,newY,state->board[x-1][newY],NONE));
    }
    if(onBoard2(x+1) && state->board[x+1][newY] != NONEPIECE && state->board[x+1][newY]->color != pawn->color) //Capture diagonally
    {
        if(promote)
        {
            addLegalMove(legalMoves, makeMove(x,y,x+1,newY,state->board[x+1][newY],QUEEN));
            addLegalMove(legalMoves,makeMove(x,y,x+1,newY,state->board[x+1][newY],KNIGHT));
        }
        else
            addLegalMove(legalMoves,makeMove(x,y,x+1,newY,state->board[x+1][newY],NONE));
    }
    if((direction == y || (direction == -1 && y == 6)) && state->board[x][y+direction] == NONEPIECE && state->board[x][y+direction*2] == NONEPIECE) //Can move twice on the first move
        addLegalMove(legalMoves,makeMove(x,y,x,y+direction*2,NONEPIECE,PAWN)); //Pawn promotion = pawn moved two steps this turn
    else { //Now check for en pessant moves
        if(onBoard2(x-1) && state->board[x-1][y]->info == 1) //The piece next to it is en-passant-able
            addLegalMove(legalMoves,makeMove(x,y,x-1,newY,state->board[x-1][y],KING)); //King promotion = kill by en pessant
        if(onBoard2(x+1) && state->board[x+1][y]->info == 1) //The piece next to it is en-passant-able
            addLegalMove(legalMoves,makeMove(x,y,x+1,newY,state->board[x+1][y],KING)); //King promotion = kill by en pessant
    }
}

void getKnightMoves(GameState *state, Piece *knight, LegalMoves *legalMoves)
{
    for(int i = 0; i<8; ++i) {
        uint8 newX = knight->x+horseJumps[i][0];
        uint8 newY = knight->y+horseJumps[i][1];
        if(onBoard(newX,newY) && state->board[newX][newY]->color != knight->color)
            addLegalMove(legalMoves,makeMove(knight->x,knight->y,newX,newY,state->board[newX][newY],NONE));
    }
}

void getBishopMoves(GameState *state, Piece *bishop, LegalMoves *legalMoves)
{
    for(int i = 0; i < 4; ++i)
    {
        uint8 stepX = diagonals[i][0];
        uint8 stepY = diagonals[i][1];
        uint8 x = bishop->x + stepX;
        uint8 y = bishop->y + stepY;
        while(onBoard(x,y)) {
            if(state->board[x][y]->color != bishop->color)
                addLegalMove(legalMoves,makeMove(bishop->x,bishop->y,x,y,state->board[x][y],NONE));
            if(state->board[x][y] != NONEPIECE)
                break; //Stop searching in the direction at the first piece hit.
            x+=stepX;
            y+=stepY;
        }
    }
}

void getRookMoves(GameState *state, Piece *rook, LegalMoves *legalMoves)
{
    for(int i = 0; i < 4; ++i)
    {
        uint8 stepX = nonDiagonals[i][0];
        uint8 stepY = nonDiagonals[i][1];
        uint8 x = rook->x + stepX;
        uint8 y = rook->y + stepY;
        while(onBoard(x,y)) {
            if(state->board[x][y]->color != rook->color)
                addLegalMove(legalMoves,makeMove(rook->x,rook->y,x,y,state->board[x][y],(rook->info)*BISHOP));

            if(state->board[x][y] != NONEPIECE)
                break; //Stop searching the lane/file at first piece hit.
            x+=stepX;
            y+=stepY;
        }
    }
}

inline void getQueenMoves(GameState *state, Piece *queen, LegalMoves *legalMoves)
{
    getBishopMoves(state,queen,legalMoves);
    getRookMoves(state,queen,legalMoves);
}

void applyMove(GameState *state, Move *move)
{
    uint8 fromX = move->fromX;
    uint8 fromY = move->fromY;
    uint8 toX = move->toX;
    uint8 toY = move->toY;
    Piece *mover = state->board[fromX][fromY];

    state->board[toX][toY] = mover; //Move piece on board
    mover->x = toX;
    mover->y = toY;
    state->board[fromX][fromY] = NONEPIECE; //Replace old position with NONE

    if(move->promotion != NONE) //Check flags
    {
        if(move->promotion == PAWN)
        {
            mover->info = 1;
            state->enPassant = mover;
        }
        else if(move->promotion == BISHOP)
            mover->info = 0; //Remove castling rights
        else if(move->promotion == KNIGHT)
            mover->type = KNIGHT;
        else if(move->promotion == QUEEN)
            mover->type = QUEEN;
        else if(move->promotion == KING)
            state->board[move->dead->x][move->dead->y] = NONEPIECE;
    }
    if(move->dead != NONEPIECE) //Something died
    {
        Piece **enemyPieces = state->turn == WHITE ? state->blackPieces : state->whitePieces;
        uint8 *liveEnemies = state->turn == WHITE ? &state->liveBlack : &state->liveWhite;
        *liveEnemies = *liveEnemies -1;
        for(int i = 1; i < *liveEnemies; ++i)
        {
            if(enemyPieces[i] == move->dead) { //Put the dead piece to the back of list
                enemyPieces[i] = enemyPieces[*liveEnemies]; //Put back of list to front
                enemyPieces[*liveEnemies] = move->dead; //Put dead piece to back of list
                break;
            }
        }
    }
    else if(state->board[toX][toY]->type == KING && abs(fromX-toX) == 2) {  //Castling
        Piece *rook = state->board[(fromX<toX)*7][fromY];
        rook->x = (fromX<toX)*2+3;
        state->board[rook->x][fromY] = rook; //Move rook
        state->board[(fromX<toX)*7][fromY] = NONEPIECE; //Replace rook's old position with None
    }
    state->enPassant->info = 0; //Remove old en passant info
    state->turn = 1-state->turn;
    state->movesMade[state->moveCounter++] = move;
}

void undoMove(GameState *state)
{
    Move *move = state->movesMade[--state->moveCounter];
    uint8 fromX = move->fromX;
    uint8 fromY = move->fromY;
    uint8 toX = move->toX;
    uint8 toY = move->toY;
    Piece *mover = state->board[toX][toY];

    state->board[fromX][fromY] = mover; //Move piece on board back
    mover->x = fromX;
    mover->y = fromY;
    state->board[toX][toY] = NONEPIECE; //Replace position with NONE

    if(move->dead != NONEPIECE) {
        state->turn == WHITE ? state->liveWhite++ : state->liveBlack++;
        state->board[move->dead->x][move->dead->y] = move->dead; //Bring back to life the dead piece
    } else if(mover->type == KING && abs(fromX-toX) == 2) { //Castle
        Piece *rook = state->board[(fromX<toX)*2+3][fromY];
        state->board[(fromX<toX)*7][fromY] = rook; //Move rook back
        rook->x = (fromX<toX)*7;
        state->board[(fromX<toX)*2+3][fromY] = NONEPIECE;
    }
    if(move->promotion != NONE)
    {
        if(move->promotion == PAWN)
            mover->info = 0;
        else if(move->promotion == BISHOP) //King or rook lost it's castling right by that move, now restore it
            mover->info = 1;
        else if(move->promotion == QUEEN || move->promotion == KNIGHT) //Pawn was promoted
            mover->type = PAWN; //Make it a pawn again
        else if(move->promotion == KING) //En passant happened, make the pawn able to be en passanted again
        {
            move->dead->info = 1;
            state->enPassant = move->dead;
        }
    }
    state->turn = 1 - state->turn;
}

inline int utility(GameState *state)
{
    Piece *king = state->turn ? state->blackPieces[0] : state->whitePieces[0];
    if(tileAttacked(state,king->x,king->y))
        return state->turn*MAX_INTEGER + (1-state->turn)*MIN_INTEGER;
    return 0; //Draw
}


