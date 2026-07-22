/***********************************************************************************************
*
*   chess.c - A simple chess engine implementation.
*
*   Copyright (c) 2026 Josh Hayden (@jhayden02)
*
*   This is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License v3.0 as published
*   by the Free Software Foundation.
*  
*   This is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*  
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
***********************************************************************************************/

#include <stddef.h>
#include <stdlib.h>

#include "raylib.h"

#define GAME_HEIGHT 852
#define GAME_WIDTH 852
#define GAME_FPS 60

#define BOARD_ROWS 8
#define BOARD_COLS BOARD_ROWS
#define BOARD_SQUARE_SIZE 96

// For non-square margins of the board.
#define BOARD_X_OFFSET 42
#define BOARD_Y_OFFSET (BOARD_SQUARE_SIZE + BOARD_X_OFFSET)

// The pixel locations of the board edges.
#define BOARD_LEFT_X BOARD_X_OFFSET
#define BOARD_BOTTOM_Y (GAME_HEIGHT - BOARD_Y_OFFSET + BOARD_SQUARE_SIZE)                                                                                                          
// Centers the piece inside the square instead of sitting at the bottom.
#define PIECE_Y_OFFSET 6

#define SELECTION_WHITE (Color){255, 255, 255, 50}

typedef enum {
    NO_PIECE,
    PIECE_WHITE_PAWN,
    PIECE_WHITE_KNIGHT,
    PIECE_WHITE_BISHOP,
    PIECE_WHITE_ROOK,
    PIECE_WHITE_QUEEN,
    PIECE_WHITE_KING,
    PIECE_BLACK_PAWN,
    PIECE_BLACK_KNIGHT,
    PIECE_BLACK_BISHOP,
    PIECE_BLACK_ROOK,
    PIECE_BLACK_QUEEN,
    PIECE_BLACK_KING
} piece_type;

typedef struct {
    piece_type piece;
    Rectangle rec;
} square_type;

square_type board[BOARD_ROWS][BOARD_COLS];

// Returns a rectangle of a square given its column and row.
Rectangle get_board_rec(int row, int col)
{
    int x = (col * BOARD_SQUARE_SIZE) + BOARD_X_OFFSET;
    int y = GAME_HEIGHT - (row * BOARD_SQUARE_SIZE) - BOARD_Y_OFFSET;
    return (Rectangle){x, y, BOARD_SQUARE_SIZE, BOARD_SQUARE_SIZE};
}

// Returns the row number that a y coordinate exists in.
int get_board_row(int y)
{
    return (BOARD_BOTTOM_Y - y) / BOARD_SQUARE_SIZE;
}            

// Returns the column number that an x coordinate exists in.
int get_board_col(int x) {
    return (x - BOARD_LEFT_X)  / BOARD_SQUARE_SIZE;
}

bool is_valid_move(square_type *origin_square, square_type *destination_square)
{
    (void)origin_square;
    (void)destination_square;
    return true;
}

void init_board(void)
{
    // Initialize every square first to be empty.
    for (int col = 0; col < BOARD_COLS; col++) {
        for (int row = 0; row < BOARD_ROWS; row++) {
           board[row][col].piece = NO_PIECE;
           board[row][col].rec = get_board_rec(row, col);
        } 
    }
    
    // Initialize all the starting piece positions.
    for (int col = 0; col < BOARD_COLS; col++) {
        board[1][col].piece = PIECE_WHITE_PAWN;
        board[6][col].piece = PIECE_BLACK_PAWN;
    }

    piece_type back_rank[BOARD_COLS] = {
        PIECE_WHITE_ROOK,
        PIECE_WHITE_KNIGHT,
        PIECE_WHITE_BISHOP,
        PIECE_WHITE_QUEEN,
        PIECE_WHITE_KING,
        PIECE_WHITE_BISHOP,
        PIECE_WHITE_KNIGHT,
        PIECE_WHITE_ROOK
    };

    for (int col = 0; col < BOARD_COLS; col++) {
        board[0][col].piece = back_rank[col];
        // Black enums sit one full color-block past the white ones.
        board[7][col].piece = back_rank[col] + (PIECE_BLACK_PAWN - PIECE_WHITE_PAWN);
    }
}

void draw_pieces(Texture2D *t_pieces)
{
    for (int row = 0; row < BOARD_ROWS; row++) {
    for (int col = 0; col < BOARD_COLS; col++) {
        const Rectangle piece_texture_rec = {
            board[row][col].piece * BOARD_SQUARE_SIZE,
            0.0f,
            BOARD_SQUARE_SIZE,
            BOARD_SQUARE_SIZE
        };

        Vector2 position = {
            board[row][col].rec.x,
            board[row][col].rec.y - PIECE_Y_OFFSET
        };

        DrawTextureRec(
            *t_pieces,
            piece_texture_rec,
            position,
            WHITE
        );
    }
    } 
}

void draw_overlays(square_type **selected_square)
{
    if (*selected_square == NULL) {
        return;
    }
    // Draw white square on selected square.
    DrawRectangleRec((*selected_square)->rec, SELECTION_WHITE);
    DrawRectangleLinesEx((*selected_square)->rec, 4, WHITE);
}

void update_board(square_type **selected_square)
{
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return;
    }

    Vector2 mouse_pos = GetMousePosition();
    int row = get_board_row((int)mouse_pos.y);
    int col = get_board_col((int)mouse_pos.x);
    square_type *clicked_square = &board[row][col];
    
    // No piece currently selected.
    if (*selected_square == NULL) {
        if (clicked_square->piece != NO_PIECE) {
            *selected_square = clicked_square;
        }
        return;
    }

    // The same piece is selected again. Deselect.
    if (*selected_square == clicked_square) {
        *selected_square = NULL;
        return;
    }
    
    if (is_valid_move(*selected_square, clicked_square)) {
        // A valid square was clicked. Move the selected piece there.
        clicked_square->piece = (*selected_square)->piece;
        (*selected_square)->piece = NO_PIECE;
        *selected_square = NULL;
    }
}

int main(void)
{ 
    SetTraceLogLevel(LOG_DEBUG);

    init_board();

    square_type *selected_square = NULL;

    InitWindow(GAME_WIDTH, GAME_HEIGHT, "Chess");
    // Load all textures. Must be after OpenGL context is initialized (InitWindow).
    Image i_chess_board = LoadImage("res/board.png");
    Image i_pieces = LoadImage("res/pieces.png");

    Texture2D t_board = LoadTextureFromImage(i_chess_board);
    Texture2D t_pieces = LoadTextureFromImage(i_pieces);

    UnloadImage(i_chess_board);
    UnloadImage(i_pieces); SetTargetFPS(GAME_FPS);
    while (!WindowShouldClose())
    {
        // Update step.
        update_board(&selected_square);

        // Draw step.
        BeginDrawing();
        ClearBackground(RAYWHITE); 

        // Draw the board.
        DrawTexture(t_board, 0, 0, WHITE);
        draw_pieces(&t_pieces); 
        draw_overlays(&selected_square);
        EndDrawing();
    }

    UnloadTexture(t_board);
    UnloadTexture(t_pieces);

    CloseWindow();
    return 0;
}
