#ifndef TEXTURE_MANAGER
#define TEXTURE_MANAGER

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "libs/collections.h"
#include "libs/vector.h"

Textures textures;

bool load(char* path, char* id, SDL_Renderer* renderer);
void draw(char* id, Vec2f pos, int width, int height, SDL_Renderer* renderer, SDL_RendererFlip flip);
void drawFrame(char* id, Vec2f pos, int width, int height, int currentRow, int currentFrame, SDL_Renderer* renderer, SDL_RendererFlip flip);

#endif
