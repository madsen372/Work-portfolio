#ifndef __Utils_h
#define __Utils_h

#include "Game.h"
#include "Piece.h"
#include "Move.h"

void printBoard(GameState *state);
void printPiece(Piece *piece);
void printMove(Move *move);
void printType(Type type);

#endif