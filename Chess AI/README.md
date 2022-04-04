First compile the program using:

gcc Piece.c Move.c Game.c Main.c Utils.c -O2


If you are on linux, you should be able to run it with the command: ./a
If you are on Windows, you should be able to run it with the command: a.exe

You can only play as black, and the chess AI will think for about 10 seconds each move, 
most moves being faster and a few slower.

The lower left corner of the chessboard is indexed 0,0
The lower right corner of the chessboard is indexed 7,0
The upper left corner of the chessboard is indexed 0,7
The upper right corner of the chessboard is indexed 7,7

Just run the program, there are a few instructions being printed. :) 