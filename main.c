
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "game.h"
#include "render.h"
#include <SDL.h>

#define SCREEN_TITLE "Minesweeper"

#define SS_TILES_PATH "spritesheet_tiles.png"
#define SS_FACES_PATH "spritesheet_faces.png"
#define SS_SCORE_PATH "spritesheet_score.png"

int main(int argc, char* argv[])
{

    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;
    SDL_Window* gWindow = NULL;
    SDL_Renderer* gRenderer = NULL;
    render_init(&gWindow, &gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE);

    // Load textures
    LTexture spritesheet_tiles, spritesheet_faces, spritesheet_score;
    LTexture_init(&spritesheet_tiles); LTexture_init(&spritesheet_faces); LTexture_init(&spritesheet_score);
    LTexture_loadFromPath(gRenderer, &spritesheet_tiles, SS_TILES_PATH);  LTexture_loadFromPath(gRenderer, &spritesheet_faces, SS_FACES_PATH);  LTexture_loadFromPath(gRenderer, &spritesheet_score, SS_SCORE_PATH);

    SDL_Rect* spritesheet_tiles_clips = LTexture_spritesheet_create_rects(16, 2, 16, 16);
    SDL_Rect* spritesheet_faces_clips = LTexture_spritesheet_create_rects(5, 1, 24, 24);
    SDL_Rect* spritesheet_score_clips = LTexture_spritesheet_create_rects(10, 1, 13, 23);

    LTexture* spritesheets[SPRITESHEET_TOTAL] = { &spritesheet_faces, &spritesheet_score, &spritesheet_tiles }; // Array of spritesheet pointers
    SDL_Rect* spritesheets_clips[SPRITESHEET_TOTAL] = { spritesheet_faces_clips, spritesheet_score_clips, spritesheet_tiles_clips }; // Array of spritesheet clips

    // Load game board
    const int BOARD_WIDTH = 21;
    const int BOARD_HEIGHT = 16;
    const int BOARD_BOMB_AMOUNT = 50;
    gameBoard gBoard = gBoard_init(BOARD_WIDTH, BOARD_HEIGHT, BOARD_BOMB_AMOUNT);
    int GAME_AREA = BOARD_WIDTH * BOARD_HEIGHT;

    // Load grid renderer
    const int GRID_CELL_SIZE = 30;

    SDL_Event e;
    bool running = true;
    int mouse_x = 0; int mouse_y = 0;
    int mouse_b = 0;
    int last_preview_x = 0; int last_preview_y = 0; // init as really big number

    printf("Gameboard Resolution: %dx%d\n", gBoard.bWidth * GRID_CELL_SIZE, gBoard.bHeight * GRID_CELL_SIZE);
    render_grid(gRenderer, &gBoard, GRID_CELL_SIZE, spritesheets, spritesheets_clips);
    while (running)
    {
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_MOUSEBUTTONDOWN:
                SDL_GetMouseState(&mouse_x, &mouse_y); // Get mouse pos
                mouse_b = e.button.button; // Check if left click is held down

                switch (mouse_b)
                {
                case SDL_BUTTON_RIGHT:
                    printf("{%d, %d} {%d, %d}: Flag requested\n", mouse_x, mouse_y, mouse_x / GRID_CELL_SIZE, mouse_y / GRID_CELL_SIZE);
                    gBoard_user_place_flag(&gBoard, mouse_x / GRID_CELL_SIZE, mouse_y / GRID_CELL_SIZE);
                    break;
                }
                break;

            case SDL_MOUSEBUTTONUP:
                SDL_GetMouseState(&mouse_x, &mouse_y); // Get mouse pos
                mouse_b = e.button.button; // Check if left click is released
                switch (mouse_b)
                {
                case SDL_BUTTON_LEFT:
                    // Check if mouse pos is in window
                    if (mouse_x < 0 || mouse_x > SCREEN_WIDTH || mouse_y < 0 || mouse_y > SCREEN_HEIGHT)
                    {
                        printf("{%d, %d} {%d, %d}: Mouse out of window\n", mouse_x, mouse_y, mouse_x / GRID_CELL_SIZE, mouse_y / GRID_CELL_SIZE);
                        break;
                    }

                    printf("{%d, %d} {%d, %d}: Mouse released\n", mouse_x, mouse_y, mouse_x / GRID_CELL_SIZE, mouse_y / GRID_CELL_SIZE);
                    gBoard_user_click(&gBoard, mouse_x / GRID_CELL_SIZE, mouse_y / GRID_CELL_SIZE);
                    break;
                }

                if (gBoard.GAME_STATE == GAME_STATE_DEAD)
                {
                    printf("You Died!\n");
                    gBoard_dead(&gBoard);

                }
                else if ((gBoard.bWidth * gBoard.bHeight) - gBoard.bBombAmount == gBoard.GAME_TILES_SHOWN)
                {
                    printf("Game Won!\n");
                }

                break;


            }

            render_grid(gRenderer, &gBoard, GRID_CELL_SIZE, spritesheets, spritesheets_clips); // Render grid after all events have been handled
        }
    }

    return 0;
}