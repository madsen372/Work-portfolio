#ifndef __Move_h
#define __Move_h

#include "MACROS.h"
#include "Piece.h"

typedef struct Move
{
    uint8 fromX;
    uint8 fromY;
    uint8 toX;
    uint8 toY;
    Piece *dead;        //The unit that died (might be NONEPIECE)
    Type promotion;     //Because of pawns, this additional info must be stored
} Move;

Move *makeMove(uint8 fromX, uint8 fromY, uint8 toX, uint8 toY, Piece* dead, Type promotion);
void freeMove(Move *move);

#endif