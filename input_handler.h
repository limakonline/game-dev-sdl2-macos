#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "types.h"

bool joystickButtons[16];
bool keyboardKeys[200];

enum STICK {
    L_HOR = 0,
    L_VER,
    R_HOR,
    R_VER
};

void input_handler(Game* g);
void update(void);
void input_clean(void);

// ====== HANDLE JOYSTICKS
typedef struct {
    SDL_Joystick** controllers;
    int capacity;
    int count;
} Controllers;

Controllers* controllers;
bool joysticks_initialised;

void initialiseJoysticks(void);
bool joysticksInitialised(void);
int currentJoystickId(void);
int deadZoneArea(void);
void displayPressedButtons(void);

#endif
