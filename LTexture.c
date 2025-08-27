
#include <SDL.h>
#include <SDL_Image.h>
#include <stdbool.h>
#include <stdio.h>

struct LTexture {
    SDL_Texture* mTexture; // Actual hardware texture
    int mWidth, mHeight; // Image diemnsions
}; typedef struct LTexture LTexture;

SDL_Rect* LTexture_spritesheet_create_rects(int tiles, int layers, int tile_w, int tile_h)
{
    SDL_Rect* tile_array = malloc(tiles * sizeof(SDL_Rect));
    int tiles_p_layer = tiles / layers; // tiles per layer
    int count = 0;

    for (int i = 0; i < layers; i++)
    {
        for (int j = 0; j < tiles_p_layer; j++)
        {
            tile_array[count].x = j * tile_w;
            tile_array[count].y = i * tile_h;

            tile_array[count].w = tile_w;
            tile_array[count].h = tile_h;
            count++;
        }
    }

    return tile_array;
}

void LTexture_init(LTexture* texture)
{
    // init a LTexture struct
    texture->mTexture = NULL;
    texture->mWidth = 0;
    texture->mHeight = 0;
}

bool LTexture_loadFromPath(SDL_Renderer* gRenderer, LTexture* texture, char* path)
{
    bool success = true; // Success flag

    // Final texture
    SDL_Texture* newTexture = NULL;

    // Load image at path
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        success = false;
    }
    else
    {
        // Create texture from surface (loadedSurface) pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
            success = false;
        }
        else
        {
            // Get image dimensions 
            texture->mWidth = loadedSurface->w;
            texture->mHeight = loadedSurface->h;
        }
        SDL_FreeSurface(loadedSurface);
    }

    // Return
    texture->mTexture = newTexture;
    return success;
}

void LTexture_render(SDL_Renderer* gRenderer, LTexture* texture, int x, int y, SDL_Rect* clip, int GRID_CELL_SIZE)
{
    SDL_Rect renderQuad = { x, y, GRID_CELL_SIZE, GRID_CELL_SIZE };

    SDL_RenderCopy(gRenderer, texture->mTexture, clip, &renderQuad);
}

int LTexture_getWidth(LTexture* texture)
{
    return texture->mWidth;
}

int LTexture_getHeight(LTexture* texture)
{
    return texture->mHeight;
}