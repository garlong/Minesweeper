#ifndef LTexture_h
#define LTexture_h

#include <SDL.h>
#include <stdbool.h>

struct LTexture {
    SDL_Texture* mTexture; // Actual hardware texture
    int mWidth, mHeight; // Image diemnsions
}; typedef struct LTexture LTexture;

/*
    Init a LTexture structure.
    When initing the structure for the first time, set it to NULL.
*/
void LTexture_init(LTexture* texture);

/*
    Load a image from a path into a texture.
    Sets LTexture->mTexture.
*/
bool LTexture_loadFromPath(SDL_Renderer* gRenderer, LTexture* texture, char* path);

/*
    Render the LTexture onto the screen
*/
void LTexture_render(SDL_Renderer* gRenderer, LTexture* texture, int x, int y, SDL_Rect* clip, int GRID_CELL_SIZE);

/*
    Create the rect clips for spritesheets when given a LTexture containing a spritesheet
    - spritesheet: The LTexture containing the spritesheet
    - tiles: The number of tiles the spritesheet has
    - layers: The amount of tiles per height in the tilesheet (A 3x4 spritesheet would have a layer of 4)
    - tile_w: The width of a tile
    - tile_h: The height of a tile

    Returns an array of SDL_Rects that are the clips in the order of the spritesheet.
*/
SDL_Rect* LTexture_spritesheet_create_rects(int tiles, int layers, int tile_w, int tile_h);

#endif