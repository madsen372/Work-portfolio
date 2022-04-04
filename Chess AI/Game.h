#ifndef __GameState_h
#define __GameState_h
#include "MACROS.h"
#include "Piece.h"
#include "Move.h"

extern const int MIN_INTEGER;
extern const int MAX_INTEGER;

typedef struct GameState
{
    Color turn;                 //0 is white, 1 is black
    uint16 moveCounter;         //Number of moves actually made
    uint8 liveWhite;            //White's number of live pieces: [1;16]
    uint8 liveBlack;            //Black's number of live pieces: [1;16]
    Move *movesMade[10000];      //Quite unlikely to have games with more than 5000 moves (10000 ply)
    Piece *board[8][8];         //The board is just a pointer to 64 pieces
    Piece *whitePieces[16];     //A list of white's pieces
    Piece *blackPieces[16];     //A list of black's pieces
    Piece *enPassant;           //A pointer to the piece, that can be en passant'ed
} GameState;

typedef struct LegalMoves
{
    uint16 count;     //The amount of legal moves
    Move* moves[230]; //It seems that chess has a maximal branching factor of about 224
} LegalMoves;

GameState *newGame();

LegalMoves *legalMoves(GameState *state);
uint8 onBoard(uint8 x, uint8 y);
uint8 onBoard2(uint8 x);
Piece *rayCast(GameState *state, uint8 x, uint8 y, uint8 stepX, uint8 stepY);
uint8 canAttack(Piece *atk, Piece *def);
void getKingAttackers(GameState *state, Piece **attackersList);
uint8 tileAttacked(GameState *state, uint8 x, uint8 y);
void addLegalMove(LegalMoves* legalMoves, Move* move);


void getKingMoves(GameState *state, Piece *king, LegalMoves *legalMoves, Piece **kingAttackers);
void getPawnMoves(GameState *state, Piece *pawn, LegalMoves *legalMoves);
void getKnightMoves(GameState *state, Piece *knight, LegalMoves *legalMoves);
void getBishopMoves(GameState *state, Piece *bishop, LegalMoves *legalMoves);
void getRookMoves(GameState *state, Piece *rook, LegalMoves *legalMoves);
void getQueenMoves(GameState *state, Piece *queen, LegalMoves *legalMoves);

void applyMove(GameState *state, Move *move); //Apply a move to the board
void undoMove(GameState *state); //Undos the last move

int gameOver(GameState *state, LegalMoves *lm);
int utility(GameState* state);

#endif
