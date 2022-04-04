#include "MACROS.h"
#include "Move.h"

static int makeMoveDebug = 0;
static int freeMoveDebug = 0;

Move *makeMove(uint8 fromX, uint8 fromY, uint8 toX, uint8 toY, Piece *dead, Type promotion)
{
    MALLOC(Move,newMove)
    newMove->fromX = fromX;
    newMove->fromY = fromY;
    newMove->toX = toX;
    newMove->toY = toY;
    newMove->dead = dead;
    newMove->promotion = promotion;
    return newMove;
}



inline void freeMove(Move *move)
{
    freeMoveDebug++;
    free(move);
    printf("ALLOC2: %d\n",makeMoveDebug);
    printf("FREED2: %d\n",freeMoveDebug);
}
