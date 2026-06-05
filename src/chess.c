/***********************************************************************************************
*
*   chess.c - A simple chess engine implementation.
*
*   Copyright (c) 2026 Josh Hayden (@jhayden02)
*
*   Blink's Thinks is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License v3.0 as published
*   by the Free Software Foundation.
*  
*   Blink's Thinks is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*  
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
***********************************************************************************************/

#include "raylib.h"

#define GAME_HEIGHT 852
#define GAME_WIDTH 852
#define GAME_FPS 60

#define BOARD_ROWS 8
#define BOARD_COLS BOARD_ROWS
#define BOARD_SQUARE_SIZE 96
#define BOARD_X_OFFSET 42
#define BOARD_Y_OFFSET (BOARD_SQUARE_SIZE + (BOARD_SQUARE_SIZE / 2))

typedef enum {
    SQUARE_EMPTY,
    SQUARE_WHITE_PAWN,
    SQUARE_WHITE_KNIGHT,
    SQUARE_WHITE_BISHOP,
    SQUARE_WHITE_ROOK,
    SQUARE_WHITE_QUEEN,
    SQUARE_WHITE_KING,
    SQUARE_BLACK_PAWN,
    SQUARE_BLACK_KNIGHT,
    SQUARE_BLACK_BISHOP,
    SQUARE_BLACK_ROOK,
    SQUARE_BLACK_QUEEN,
    SQUARE_BLACK_KING
} square;

square board[BOARD_ROWS][BOARD_COLS];

// White pieces are uppercase, black pieces are lowercase.
void init_board()
{
    for (int col = 0; col < BOARD_COLS; col++) {
        board[1][col] = SQUARE_WHITE_PAWN;
        board[6][col] = SQUARE_BLACK_PAWN;
    }
    
    board[0][0] = SQUARE_WHITE_ROOK;
    board[0][7] = SQUARE_WHITE_ROOK;

    board[7][0] = SQUARE_BLACK_ROOK;
    board[7][7] = SQUARE_BLACK_ROOK;

    board[0][1] = SQUARE_WHITE_KNIGHT;
    board[0][6] = SQUARE_WHITE_KNIGHT;
    
    board[7][1] = SQUARE_BLACK_KNIGHT;
    board[7][6] = SQUARE_BLACK_KNIGHT;
    
    board[0][2] = SQUARE_WHITE_BISHOP;
    board[0][5] = SQUARE_WHITE_BISHOP;
    
    board[7][2] = SQUARE_BLACK_BISHOP;
    board[7][5] = SQUARE_BLACK_BISHOP;

    board[0][3] = SQUARE_WHITE_QUEEN;
    board[0][4] = SQUARE_WHITE_KING;
    
    board[7][3] = SQUARE_BLACK_QUEEN;
    board[7][4] = SQUARE_BLACK_KING;
}

int main(void)
{ 
    InitWindow(GAME_WIDTH, GAME_HEIGHT, "Chess");

    Image i_chess_board = LoadImage("res/board.png");
    Image i_pieces = LoadImage("res/pieces.png");

    Texture2D t_chess_board = LoadTextureFromImage(i_chess_board);
    Texture2D t_pieces = LoadTextureFromImage(i_pieces);

    UnloadImage(i_chess_board);
    UnloadImage(i_pieces);

    SetTargetFPS(GAME_FPS);

    init_board();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Draw board and pieces.
        DrawTexture(t_chess_board, 0, 0, WHITE);
        for (int row = 0; row < BOARD_ROWS; row++) {
            for (int col = 0; col < BOARD_COLS; col++) {
                const square current_square = board[row][col];
                const Rectangle piece_rec = {
                    current_square * BOARD_SQUARE_SIZE,
                    0.0f,
                    BOARD_SQUARE_SIZE,
                    BOARD_SQUARE_SIZE
                };
                const Vector2 position = {
                    (col * BOARD_SQUARE_SIZE) + BOARD_X_OFFSET,
                    GAME_HEIGHT - (row * BOARD_SQUARE_SIZE) - BOARD_Y_OFFSET
                };
                DrawTextureRec(t_pieces, piece_rec, position, WHITE);
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
