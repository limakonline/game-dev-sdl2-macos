#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdbool.h>

#include "texture_manager.h"
#include "libs/vector.h"
#include "types.h"
#include "input_handler.h"
#include "buttons.h"

#define WINDOW_W 640
#define WINDOW_H 480

const int FPS = 60;
const int DELAY_TIME = 1000.0f / FPS;

// TODO move this struct to something like global.h file
// so it can be accessed and changed in different places
Game g = {
    .window = 0,
    .renderer = 0,
    .quit = false,
    .currentState = MENU,
    .mouse = {
        .position = {0, 0},
        .clicked = false
    }
};

GameObject p = {
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
    .width = 32,
    .height = 32
};

GameObject e = {
    .defaultVelocity = 0,
    .currentFrame = 0,
    .animationSpritesheet = "char.png",
    .position = {
        .x = 200.0,
        .y = 200.0
    },
    .velocity = {
        .x = 0,
        .y = 0
    },
    .width = 32,
    .height = 32
};

void play_btn_callback(void) {
    g.currentState = PLAY;
}

void exit_btn_callback(void) {
    g.quit = true;
}

void restart_btn_callback(void) {
    g.currentState = MENU;
}

Btn play_btn = {
    .id = "play_btn",
    .spriteSheet = "play_btn.png",
    .currentState = MOUSE_OUT,
    .position = {0, 0},
    .dimension = {128, 32},
    .callback = play_btn_callback
};

Btn exit_btn = {
    .id = "exit_btn",
    .spriteSheet = "exit_btn.png",
    .currentState = MOUSE_OUT,
    .position = {0, 40},
    .dimension = {128, 32},
    .callback = exit_btn_callback
};

Btn restart_btn = {
    .id = "restart",
    .spriteSheet = "restart_btn.png",
    .currentState = MOUSE_OUT,
    .position = {0, 0},
    .dimension = {128, 32},
    .callback = restart_btn_callback
};

bool check_collision(GameObject p, GameObject e) {
    int pLeft = p.position.x;
    int pRight = p.position.x + p.width;
    int pTop = p.position.y;
    int pBottom = p.position.y + p.height;

    int eLeft = e.position.x;
    int eRight = e.position.x + e.width;
    int eTop = e.position.y;
    int eBottom = e.position.y + e.height;

    if (pLeft >= eRight) { return false; }
    if (pRight <= eLeft) { return false; }
    if (pTop >= eBottom) { return false; }
    if (pBottom <= eTop) { return false; }

    return true;
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
    load(restart_btn.spriteSheet, restart_btn.id, g.renderer);

    load(p.animationSpritesheet, "player", g.renderer);
    load(e.animationSpritesheet, "enemy", g.renderer);
    load("game_over.png", "game_over", g.renderer);

    play_btn.position = addVec2f(play_btn.position, vec2f(WINDOW_W / 2, WINDOW_H / 2));
    exit_btn.position = addVec2f(exit_btn.position, vec2f(WINDOW_W / 2, WINDOW_H / 2));
    play_btn.position.x -= play_btn.dimension.x / 2;
    play_btn.position.y -= play_btn.dimension.y / 2;

    exit_btn.position.x -= exit_btn.dimension.x / 2;
    exit_btn.position.y -= exit_btn.dimension.y / 2;

    restart_btn.position = addVec2f(restart_btn.position, vec2f(WINDOW_W / 2, WINDOW_H / 2));
    restart_btn.position.x -= restart_btn.dimension.x / 2;
    restart_btn.position.y += 128;

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
    } else {
        if(keyboardKeys[SDLK_w]) {
            p.velocity.y = -p.defaultVelocity;
        }

        if(keyboardKeys[SDLK_s]) {
            p.velocity.y = p.defaultVelocity;
        }

        if(keyboardKeys[SDLK_a]) {
            p.velocity.x = -p.defaultVelocity;
        }

        if(keyboardKeys[SDLK_d]) {
            p.velocity.x = p.defaultVelocity;
        }
    }
}

void update_btn_state(Btn* btn) {
    if(is_mouse_over_btn(*btn, g.mouse.position) && g.mouse.clicked) {
        btn->currentState = CLICKED;
        btn->callback();
    } else if(is_mouse_over_btn(*btn, g.mouse.position)) {
        btn->currentState = MOUSE_OVER;
    } else {
        btn->currentState = MOUSE_OUT;
    }
}

// TODO: maybe move it to different files like menu_state/play_state/etc.
void menu_update(void) {
    update_btn_state(&play_btn);
    update_btn_state(&exit_btn);
}

void play_update(void) {
    if (check_collision(p, e)) {
        g.currentState = OVER;
    }
    p.velocity.x = 0;
    p.velocity.y = 0;

    displayPressedButtons();
    move_player();

    p.position = addVec2f(p.position, p.velocity);
}

void over_update(void) {
    update_btn_state(&restart_btn);
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
    drawFrame("player", p.position, 32, 32, 1, p.frameNumber, g.renderer, SDL_FLIP_HORIZONTAL);
    draw("enemy", e.position, 32, 32, g.renderer, SDL_FLIP_NONE);
}

void over_render(void) {
    int x = WINDOW_W / 2 - 128;
    int y = WINDOW_H / 2 - (256 * 3/4);
    draw("game_over", vec2f(x, y), 256, 256, g.renderer, SDL_FLIP_NONE);

    drawFrame(
            restart_btn.id,
            restart_btn.position,
            restart_btn.dimension.x,
            restart_btn.dimension.y,
            1, 
            restart_btn.currentState, g.renderer, SDL_FLIP_NONE);
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
