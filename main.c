#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdbool.h>

#include "texture_manager.h"
#include "libs/vector.h"
#include "types.h"
#include "input_handler.h"

#define WINDOW_W 640
#define WINDOW_H 480

const int FPS = 60;
const int DELAY_TIME = 1000.0f / FPS;

enum state_btn {
    MOUSE_OUT = 0,
    MOUSE_OVER = 1,
    CLICKED = 2
};

typedef struct {
    char* id;
    char* spriteSheet;
    enum state_btn currentState;
    Vec2f position;
    Vec2f dimension;
} Btn;

Btn play_btn = {
    .id = "play_btn",
    .spriteSheet = "play_btn.png",
    .currentState = MOUSE_OUT,
    .position = {0, 0},
    .dimension = {128, 32}
};

Btn exit_btn = {
    .id = "exit_btn",
    .spriteSheet = "exit_btn.png",
    .currentState = MOUSE_OUT,
    .position = {0, 40},
    .dimension = {128, 32}
};

Game g = {
    .window = 0,
    .renderer = 0,
    .quit = false,
    .currentState = PLAY,
    .mouse = {
        .position = {0, 0},
        .clicked = false
    }
};

Player p = {
    .defaultVelocity = 5,
    .currentFrame = 0,
    .animationSpritesheet = "char.png",
    .position = {
        .x = 100.0,
        .y = 100.0
    },
    .velocity = {
        .x = 0,
        .y = 0
    },
};

bool is_mouse_over_btn(Btn btn, Vec2f position) {
    float x = btn.position.x;
    float y = btn.position.y;
    float w = btn.position.x + btn.dimension.x;
    float h = btn.position.y + btn.dimension.y;

    if(position.x >= x &&
        position.x < w &&
        position.y >= y &&
        position.y < h) {
        return true;
    }

    return false;
}

bool init(const char * title, Vec2f pos, int width, int height, int flags) {
    if (SDL_Init(SDL_INIT_EVERYTHING) >= 0) {
        g.window = SDL_CreateWindow(title, pos.x, pos.y, width, height, flags);

        if(g.window != 0) {
            g.renderer = SDL_CreateRenderer(g.window, -1, 0);
        }
    } else {
        return false;
    }

    load(play_btn.spriteSheet, play_btn.id, g.renderer);
    load(exit_btn.spriteSheet, exit_btn.id, g.renderer);
    load(p.animationSpritesheet, "player", g.renderer);

    play_btn.position = addVec2f(play_btn.position, vec2f(WINDOW_W / 2, WINDOW_H / 2));
    exit_btn.position = addVec2f(exit_btn.position, vec2f(WINDOW_W / 2, WINDOW_H / 2));

    play_btn.position.x -= play_btn.dimension.x / 2;
    play_btn.position.y -= play_btn.dimension.y / 2;

    exit_btn.position.x -= exit_btn.dimension.x / 2;
    exit_btn.position.y -= exit_btn.dimension.y / 2;

    return true;
}

void game_clean(void) {
    SDL_DestroyWindow(g.window);
    SDL_DestroyRenderer(g.renderer);
    SDL_Quit();
}

void move_player(void) {
    if(controllers != NULL) {
        int one_lr = SDL_JoystickGetAxis(controllers->controllers[currentJoystickId()], L_HOR);
        int one_ud = SDL_JoystickGetAxis(controllers->controllers[currentJoystickId()], L_VER);

        // TODO: upate this shit(?)
        if(one_lr > 10000) {
            p.velocity.x = p.defaultVelocity;
        }

        if(one_lr < -10000) {
            p.velocity.x = -p.defaultVelocity;
        }

        if(one_ud > 10000) {
            p.velocity.y = p.defaultVelocity;
        }

        if(one_ud < -10000) {
            p.velocity.y = -p.defaultVelocity;
        }
    }
}

// TODO: maybe move it to different files like menu_state/play_state/etc.
void menu_update(void) {
    if(is_mouse_over_btn(play_btn, g.mouse.position) && g.mouse.clicked) {
        play_btn.currentState = CLICKED;
    } else if(is_mouse_over_btn(play_btn, g.mouse.position)) {
        play_btn.currentState = MOUSE_OVER;
    } else {
        play_btn.currentState = MOUSE_OUT;
    }

    if(is_mouse_over_btn(exit_btn, g.mouse.position) && g.mouse.clicked) {
        exit_btn.currentState = CLICKED;
    } else if(is_mouse_over_btn(exit_btn, g.mouse.position)) {
        exit_btn.currentState = MOUSE_OVER;
    } else {
        exit_btn.currentState = MOUSE_OUT;
    }
}

void play_update(void) {
    p.velocity.x = 0;
    p.velocity.y = 0;

    displayPressedButtons();
    move_player();

    p.position = addVec2f(p.position, p.velocity);
}

void over_update(void) {
}

void update(void) {
    switch (g.currentState) {
        case MENU:
            menu_update();
            break;
        case PLAY:
            play_update();
            break;
        case OVER:
            over_update();
            break;
    }
}

void menu_render(void) {
    drawFrame(
            play_btn.id,
            play_btn.position,
            play_btn.dimension.x,
            play_btn.dimension.y,
            1,
            play_btn.currentState, 
            g.renderer, SDL_FLIP_NONE);

    drawFrame(
            exit_btn.id,
            exit_btn.position,
            exit_btn.dimension.x, 
            exit_btn.dimension.y,
            1, 
            exit_btn.currentState, g.renderer, SDL_FLIP_NONE);
}

void play_render(void) {
    p.frameNumber = (int) ((SDL_GetTicks() / 100) % 4);
    drawFrame("player", p.position, 32, 32, 1, p.frameNumber, g.renderer, SDL_FLIP_NONE);
}

void over_render(void) {
}

void render(void) {
    SDL_SetRenderDrawColor(g.renderer, 18, 18, 18, 255);
    SDL_RenderClear(g.renderer);

    // TODO random segfault here
    switch (g.currentState) {
        case MENU:
            menu_render();
            break;
        case PLAY:
            play_render();
            break;
        case OVER:
            over_render();
            break;
    }

    SDL_RenderPresent(g.renderer);
}

int main(void) {
    uint32_t frameStart, frameTime;

    bool windowReady = init(
            "SDL Init method", 
            vec2f(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED), 
            WINDOW_W, WINDOW_H,
            SDL_WINDOW_SHOWN);

    if(windowReady) {
        initialiseJoysticks();
        while (!g.quit) {
            frameStart = SDL_GetTicks();
            input_handler(&g);
            update();

            render();

            frameTime = SDL_GetTicks() - frameStart;

            if(frameTime < DELAY_TIME) {
               SDL_Delay((int)(DELAY_TIME - frameTime));
            }
        }
        game_clean();
        input_clean();
    }

    return 0;
}
