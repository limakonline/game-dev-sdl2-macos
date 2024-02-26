#include <SDL2/SDL.h>
#include <stdbool.h>

#include "input_handler.h"
#include "types.h"
#include "libs/vector.h"
#include "libs/collections.h"

const int joystickDeadZone = 10000;
int currentJoystick = -1;

void input_clean(void) {
    if(controllers != NULL && controllers->count > 0) {
        for(int i = 0; i < controllers->count; i++) {
            SDL_Joystick* joy = controllers->controllers[i];
            if(joy != NULL) {
                SDL_JoystickClose(joy);
            }
        }
    }
}

void displayPressedButtons(void) {
    for(int i = 0; i < (int)(sizeof(joystickButtons) / sizeof(bool)); i++) {
        if(joystickButtons[i] == true) {
            printf("Pressed: %d \n", i);
        }
    }
}

void initialiseJoysticks(void) {
    printf("INITILISATION OF JOYSTICKS\n");

    if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0) {
        SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    }

    int no_controllers = SDL_NumJoysticks();

    if (no_controllers > 0) {
        controllers = malloc(no_controllers * sizeof(Controllers*));

        for(int i = 0; i < no_controllers; i++) {
            SDL_Joystick* joy = SDL_JoystickOpen(i);

            if (joy != NULL) {
                if(i == 0) controllers->controllers = malloc(sizeof(SDL_Joystick*));
                controllers->controllers[i] = joy;
                controllers->count++;
            } else if (joy == NULL) {
                printf("Error: %s\n", SDL_GetError());
            }
        }
        SDL_JoystickEventState(SDL_ENABLE);

        printf("Initiaised %d joysticks\n", SDL_NumJoysticks());
    } else {
        printf("No joysticks connected.\n");
    }
}

void input_handler(Game* g) {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                {
                    g->quit = true;
                }
                break;
            case SDL_JOYAXISMOTION:
                {
                    currentJoystick = event.jaxis.which;
                }
                break;
            case SDL_JOYBUTTONDOWN:
                {
                    joystickButtons[event.jbutton.button] = true;
                }
                break;
            case SDL_JOYBUTTONUP:
                {
                    joystickButtons[event.jbutton.button] = false;
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    g->currentState = MENU;
                } else {
                    keyboardKeys[event.key.keysym.sym] =  true;
                }
                break;
            case SDL_KEYUP:
                    keyboardKeys[event.key.keysym.sym] =  false;
                break;
            case SDL_MOUSEMOTION:
                {
                    g->mouse.position.x = event.motion.x;
                    g->mouse.position.y = event.motion.y;
                }
            case SDL_MOUSEBUTTONDOWN:
                {
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        g->mouse.clicked = true;
                    }
                }
                break;
            case SDL_MOUSEBUTTONUP:
                {
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        g->mouse.clicked = false;
                    }
                }
                break;
        }
    }
}

int currentJoystickId(void) {
    return currentJoystick;
}

int deadZoneArea(void) {
    return joystickDeadZone;
}
