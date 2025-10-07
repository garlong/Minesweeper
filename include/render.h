#pragma once

#ifndef render_h
#define render_h

#include <SDL.h>
#include "LTexture.h"

/*
    What type of spritesheet
    - Faces spritesheet
    - Tiles spritesheet
    - Score spritesheet
*/
enum spritesheet_type {
    SPRITESHEET_FACES,
    SPRITESHEET_SCORE,
    SPRITESHEET_TILES,
    SPRITESHEET_TOTAL

}; typedef enum spritesheet_type spritesheet_type;

/*
    Faces spritesheet used for setting the rect
    - 16 Total tiles
    - 2 layers
    - Image Width: 128
    - Image Height: 32
    - Tile Width: 8
    - Tile Height: 16
*/
enum spritesheet_tiles_tile {
    SPRITESHEET_TILES_HIDDEN,
    SPRITESHEET_TILES_SHOWN,
    SPRITESHEET_TILES_FLAG,
    SPRITESHEET_TILES_QUESTION,
    SPRITESHEET_TILES_QUESTION_PRESSED,
    SPRITESHEET_TILES_MINE,
    SPRITESHEET_TILES_MINE_RED,
    SPRITESHEET_TILES_MINE_CROSS,

    SPRITESHEET_TILES_ONE,
    SPRITESHEET_TILES_TWO,
    SPRITESHEET_TILES_THREE,
    SPRITESHEET_TILES_FOUR,
    SPRITESHEET_TILES_FIVE,
    SPRITESHEET_TILES_SIX,
    SPRITESHEET_TILES_SEVEN,
    SPRITESHEET_TILES_EIGHT,
    SPRITESHEET_TILES_TOTAL
}; typedef enum spritesheet_tiles_tile spritesheet_tiles_tile;

/*
    Faces spritesheet used for setting the rect
    - 5 Total tiles
    - Image Width: 120
    - Image Height: 24
    - Tile Width: 24
    - Tile Height: 24
*/
enum spritesheet_faces_tile {
    SPRITESHEET_FACES_HAPPY,
    SPRITESHEET_FACES_HAPPY_PRESSED,
    SPRITESHEET_FACES_SUPRISED,
    SPRITESHEET_FACES_WIN,
    SPRITESHEET_FACES_LOSS,
    SPRITESHEET_FACES_TOTAL
}; typedef enum spritesheet_faces_tile spritesheet_faces_tile;

/*
    Score spritesheet used for setting the rect
    - 10 Total tiles
    - Image Width: 130
    - Image Height: 23
    - Tile Width: 13
    - Tile Height: 23
*/
enum spritesheet_score_tile {
    SPRITESHEET_SCORE_0,
    SPRITESHEET_SCORE_1,
    SPRITESHEET_SCORE_2,
    SPRITESHEET_SCORE_3,
    SPRITESHEET_SCORE_4,
    SPRITESHEET_SCORE_5,
    SPRITESHEET_SCORE_6,
    SPRITESHEET_SCORE_7,
    SPRITESHEET_SCORE_8,
    SPRITESHEET_SCORE_9,
    SPRITESHEET_SCORE_TOTAL
}; typedef enum spritesheet_score_tile spritesheet_score_tile;

/*
    Init needed SDL subsystems
    - SDL_VIDEO

    Init SDL variables
    - SDL_WINDOW
    - SDL_RENDERER

    returns true (1) on success and false (0) on fail.
*/
bool render_init(SDL_Window** gWindow, SDL_Renderer** gRenderer, int win_w, int win_h, char* win_title);

/*
    Quit out of SDL subsystems
    - SDL_Video

    Destroy SDL variables
    - SDL_RENDERER
    - SDL_WINDOW
*/
void render_quit(SDL_Window** gWindow, SDL_Renderer** gRenderer);

/*
    Render the gameboard
*/
void render_grid(SDL_Renderer* gRenderer, gameBoard* gBoard, int GIRD_CELL_SIZE, LTexture* spritesheets[], SDL_Rect* spritesheets_clips[]);

#endif