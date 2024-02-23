#include <SDL2/SDL_image.h>
#include "texture_manager.h"
#include "libs/vector.h"

bool load(char* path, char* id, SDL_Renderer* renderer) {
    SDL_Surface* tempSurface = IMG_Load(path);

    if (tempSurface == NULL) {
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);

    SDL_FreeSurface(tempSurface);

    if (texture != NULL) {
        TextureEntry entry;
        entry.key = id;
        entry.value = texture;
        texture_add(&textures, entry);
        return true;
    }

    return false;
}

void draw(char* id, Vec2f pos, int width, int height, SDL_Renderer* renderer, SDL_RendererFlip flip) {
    SDL_Rect srcRect;
    SDL_Rect destRect;

    srcRect.x = 0;
    srcRect.y = 0;
    srcRect.w = destRect.w = width;
    srcRect.h = destRect.h = height;

    destRect.x = pos.x;
    destRect.y = pos.y;

    SDL_RenderCopyEx(renderer, get(&textures, id), &srcRect, &destRect, 0, 0, flip);
}

void drawFrame(char* id, Vec2f pos, int width, int height, int currentRow, int currentFrame, SDL_Renderer* renderer, SDL_RendererFlip flip) {
    SDL_Rect srcRect;
    SDL_Rect destRect;
    srcRect.x = width * currentFrame;
    srcRect.y = height * (currentRow - 1);
    srcRect.w = destRect.w = width;
    srcRect.h = destRect.h = height;
    destRect.x = pos.x;
    destRect.y = pos.y;

    SDL_RenderCopyEx(renderer, get(&textures, id), &srcRect, &destRect, 0, 0, flip);
}
