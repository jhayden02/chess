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

#define GAME_HEIGHT 800
#define GAME_WIDTH 800
#define GAME_FPS 60

#define BOARD_ROWS 8
#define BOARD_COLS 8

#define SQUARE_PIXELS 100

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
void init_board() {
    for (int col = 0; col < BOARD_COLS; col++) {
        board[col][6] = SQUARE_BLACK_PAWN;
        board[col][1] = SQUARE_WHITE_PAWN;
    }
}

void draw_board()
{
    const int cols = GAME_WIDTH / SQUARE_PIXELS;
    const int rows = GAME_HEIGHT / SQUARE_PIXELS;

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            const bool is_dark = (x + y) % 2;
            const Color color = is_dark ? BLACK : RAYWHITE;
            const float draw_x = x * SQUARE_PIXELS;
            const float draw_y = y * SQUARE_PIXELS;
            DrawRectangle(draw_x, draw_y, SQUARE_PIXELS, SQUARE_PIXELS, color);
        }
    }
}

void update_board()
{
}

int main(void)
{ 
    InitWindow(GAME_WIDTH, GAME_HEIGHT, "chess");
    SetTargetFPS(GAME_FPS);

    init_board();

    while (!WindowShouldClose())
    {
        update_board();

        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw_board();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
