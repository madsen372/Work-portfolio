#ifndef __Piece_h
#define __Piece_h
#include "MACROS.h"

typedef enum Type
{
    NONE,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
} Type;

typedef enum Color
{
    WHITE,
    BLACK,
    NOCOLOR
} Color;

typedef struct Piece
{
    Type type;
    Color color;
    uint8 x;
    uint8 y;
    uint8 info; //If 1 then the piece is enpassant-able or it has castling rights yet.
                //The meaning of info depends on the piece.
} Piece;

Piece *NONEPIECE;

Piece *makePiece(Type type, Color color, uint8 x, uint8 y);
Piece *makePieceWithInfo(Type type, Color color, uint8 x, uint8 y, uint8 info);

#endif