
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

#include "LTexture.h"
#include "game.h"
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

bool render_init(SDL_Window** gWindow, SDL_Renderer** gRenderer, int win_w, int win_h, char* win_title)
{
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to init video subsystem! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        *gWindow = SDL_CreateWindow(win_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, win_w, win_h, SDL_WINDOW_SHOWN);
        if (*gWindow == NULL)
        {

            printf("Failed to create window! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            *gRenderer = SDL_CreateRenderer(*gWindow, -1, SDL_RENDERER_PRESENTVSYNC);
            if (*gRenderer == NULL)
            {
                printf("Failed to create renderer! SDL_Error: %s\n");
                success = false;
            }
            else
            {
                int img_flags = IMG_INIT_PNG;
                if (!(IMG_Init(IMG_INIT_PNG) & img_flags))
                {
                    printf("SDL_Image could not initalise! SDL_image Error: %s", IMG_GetError());
                    success = false;
                }
            }
        }
    }
    return success;
}

void render_quit(SDL_Window** gWindow, SDL_Renderer** gRenderer)
{
    SDL_DestroyRenderer(*gRenderer);
    SDL_DestroyWindow(*gWindow);
    *gRenderer = NULL;
    *gWindow = NULL;

    IMG_Quit();
    SDL_Quit();
}

void render_grid(SDL_Renderer* gRenderer, gameBoard* gBoard, int GRID_CELL_SIZE, LTexture* spritesheets[], SDL_Rect* spritesheets_clips[])
{
    int game_width = gBoard->bWidth;
    int game_height = gBoard->bHeight;
    int grid_width = game_width * GRID_CELL_SIZE;
    int grid_height = game_height * GRID_CELL_SIZE;

    for (int grid_y = 0; grid_y < grid_height; grid_y += GRID_CELL_SIZE)
    {
        for (int grid_x = 0; grid_x < grid_width; grid_x += GRID_CELL_SIZE)
        {
            if (gBoard->grid[grid_x / GRID_CELL_SIZE][grid_y / GRID_CELL_SIZE].flag)
            {
                LTexture_render(gRenderer, spritesheets[SPRITESHEET_TILES], grid_x, grid_y, &spritesheets_clips[SPRITESHEET_TILES][SPRITESHEET_TILES_FLAG], GRID_CELL_SIZE);
                continue;
            }
            else
                if (gBoard->grid[grid_x / GRID_CELL_SIZE][grid_y / GRID_CELL_SIZE].state == TILE_STATE_HIDDEN)
                {
                    LTexture_render(gRenderer, spritesheets[SPRITESHEET_TILES], grid_x, grid_y, &spritesheets_clips[SPRITESHEET_TILES][SPRITESHEET_TILES_HIDDEN], GRID_CELL_SIZE);
                    continue;
                }
                else
                    if (gBoard->grid[grid_x / GRID_CELL_SIZE][grid_y / GRID_CELL_SIZE].bomb)
                    {
                        LTexture_render(gRenderer, spritesheets[SPRITESHEET_TILES], grid_x, grid_y, &spritesheets_clips[SPRITESHEET_TILES][SPRITESHEET_TILES_MINE], GRID_CELL_SIZE);
                        continue;
                    }
            switch (gBoard->grid[grid_x / GRID_CELL_SIZE][grid_y / GRID_CELL_SIZE].value)
            {

            case TILE_VALUE_NOTHING:
                LTexture_render(gRenderer, spritesheets[SPRITESHEET_TILES], grid_x, grid_y, &spritesheets_clips[SPRITESHEET_TILES][SPRITESHEET_TILES_SHOWN], GRID_CELL_SIZE);
                break;

            case TILE_VALUE_ONE:
                LTexture_render(gRenderer, spritesheets[SPRITESHEET_TILES], grid_x, grid_y, &spritesheets_clips[SPRITESHEET_TILES][SPRITESHEET_TILES_ONE], GRID_CELL_SIZE);
                break;

            case TILE_VALUE_TWO:
                LTexture_render(gRenderer, spritesheets[SPRITESHEET_TILES], grid_x, grid_y, &spritesheets_clips[SPRITESHEET_TILES][SPRITESHEET_TILES_TWO], GRID_CELL_SIZE);
                break;

            case TILE_VALUE_THREE:
                LTexture_render(gRenderer, spritesheets[SPRITESHEET_TILES], grid_x, grid_y, &spritesheets_clips[SPRITESHEET_TILES][SPRITESHEET_TILES_THREE], GRID_CELL_SIZE);
                break;

            case TILE_VALUE_FOUR:
                LTexture_render(gRenderer, spritesheets[SPRITESHEET_TILES], grid_x, grid_y, &spritesheets_clips[SPRITESHEET_TILES][SPRITESHEET_TILES_FOUR], GRID_CELL_SIZE);
                break;

            case TILE_VALUE_FIVE:
                LTexture_render(gRenderer, spritesheets[SPRITESHEET_TILES], grid_x, grid_y, &spritesheets_clips[SPRITESHEET_TILES][SPRITESHEET_TILES_FIVE], GRID_CELL_SIZE);
                break;

            case TILE_VALUE_SIX:
                LTexture_render(gRenderer, spritesheets[SPRITESHEET_TILES], grid_x, grid_y, &spritesheets_clips[SPRITESHEET_TILES][SPRITESHEET_TILES_SIX], GRID_CELL_SIZE);
                break;

            case TILE_VALUE_SEVEN:
                LTexture_render(gRenderer, spritesheets[SPRITESHEET_TILES], grid_x, grid_y, &spritesheets_clips[SPRITESHEET_TILES][SPRITESHEET_TILES_SEVEN], GRID_CELL_SIZE);
                break;

            case TILE_VALUE_EIGHT:
                LTexture_render(gRenderer, spritesheets[SPRITESHEET_TILES], grid_x, grid_y, &spritesheets_clips[SPRITESHEET_TILES][SPRITESHEET_TILES_EIGHT], GRID_CELL_SIZE);
                break;
            }
        }
    }

    SDL_RenderPresent(gRenderer);
}