#include <stdio.h>
#include "Utils.h"
#include "Piece.h"
#include "Game.h"
#include "Move.h"

//Print functions
void printBoard(GameState *state)
{
    printf("\n");
    for(int y = 7; y>=0; --y)
    {
        printf("\t%d  | ",y);
        for(int x = 0; x<8; ++x)
        {
            printPiece(state->board[x][y]);
            printf(" ");
        }
        printf("|");
        if(y!=0)
            printf("\n");
    }
    printf("\n\n\t     0 1 2 3 4 5 6 7 \n\n");
}

//White pieces are printed in uppercase, and black pieces in lowercase
void printPiece(Piece *piece)
{
    int isBlack = piece->color == BLACK;
    switch(piece->type)
    {
        case 0:
            printf("-");
            break;
        case 1:
            printf("%c",'P'+isBlack*32);
            break;
        case 2:
            printf("%c",'N'+isBlack*32);
            break;
        case 3:
            printf("%c",'B'+isBlack*32);
            break;
        case 4:
            printf("%c",'R'+isBlack*32);
            break;
        case 5:
            printf("%c",'Q'+isBlack*32);
            break;
        case 6:
            printf("%c",'K'+isBlack*32);
            break;
    }
}

void printMove(Move* move)
{
    printf("---------------------------\n");
    printf("Moving from x,y:\t%d,%d\n",move->fromX,move->fromY);
    printf("Moving to x,y:  \t%d,%d\n",move->toX,move->toY);
    printf("Promotion: \t\t%d\n",move->promotion);
    if(move->dead->type != NONE)
        printf("Piece died at x,y: %d,%d\n",move->dead->x,move->dead->y);
    printf("---------------------------\n");
}

void printType(Type type)
{
    switch(type)
    {
        case NONE:
            printf("NONE\n");
            break;
        case PAWN:
            printf("PAWN\n");
            break;
        case KNIGHT:
            printf("KNIGHT\n");
            break;
        case BISHOP:
            printf("BISHOP\n");
            break;
        case ROOK:
            printf("ROOK\n");
            break;
        case QUEEN:
            printf("QUEEN\n");
            break;
        case KING:
            printf("KING\n");
            break;
    }
}