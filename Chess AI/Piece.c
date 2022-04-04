#include "MACROS.h"
#include "Piece.h"

Piece *makePiece(Type type, Color color, uint8 x, uint8 y)
{
    MALLOC(Piece,newPiece);
    newPiece->info = 0;
    newPiece->type = type;
    newPiece->color = color;
    newPiece->x = x;
    newPiece->y = y;
    return newPiece;
}

Piece *makePieceWithInfo(Type type, Color color, uint8 x, uint8 y, uint8 info)
{
    Piece* newPiece = makePiece(type,color,x,y);
    newPiece->info = info;
    return newPiece;
}