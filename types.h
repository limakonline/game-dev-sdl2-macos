#ifndef TYPES_H
#define TYPES_H

#include "libs/vector.h"

enum game_state {
    MENU = 0,
    PLAY = 1,
    OVER = 2
};

typedef struct {
    Vec2f position;
    bool clicked;
} Mouse;

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool quit;
    enum game_state currentState;
    Mouse mouse;
} Game;

typedef struct {
    float defaultVelocity;
    char* animationSpritesheet;
    int currentFrame;
    int frameNumber;
    int width;
    int height;
    Vec2f position;
    Vec2f velocity;

    SDL_Texture* texture;
    SDL_Rect sourceRect;
    SDL_Rect destRect;
} GameObject;

#endif
