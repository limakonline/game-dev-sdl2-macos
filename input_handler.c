#include <SDL2/SDL.h>
#include <stdbool.h>

#include "input_handler.h"
#include "types.h"
#include "libs/vector.h"
#include "libs/collections.h"

const int joystickDeadZone = 10000;
int currentJoystick = -1;
bool pressedButtons[16];

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
    for(int i = 0; i < (int)(sizeof(pressedButtons) / sizeof(bool)); i++) {
        if(pressedButtons[i] == true) {
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
                    pressedButtons[event.jbutton.button] = true;
                }
                break;
            case SDL_JOYBUTTONUP:
                {
                    pressedButtons[event.jbutton.button] = false;
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == 27) {
                    g->quit = true;
                }
                if (event.key.keysym.sym == 49) {
                    if (g->currentState != MENU) {
                        g->currentState = MENU;
                    }
                }
                if (event.key.keysym.sym == 50) {
                    if (g->currentState != PLAY) {
                        g->currentState = PLAY;
                    }
                }
                if (event.key.keysym.sym == 51) {
                    if (g->currentState != OVER) {
                        g->currentState = OVER;
                    }
                }
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
