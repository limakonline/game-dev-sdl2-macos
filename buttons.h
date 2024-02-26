#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdbool.h>
#include "libs/vector.h"

enum state_btn {
    MOUSE_OUT = 0,
    MOUSE_OVER = 1,
    CLICKED = 2
};

typedef void (*btn_callback)(void);

typedef struct {
    char* id;
    char* spriteSheet;
    enum state_btn currentState;
    Vec2f position;
    Vec2f dimension;
    btn_callback callback;
} Btn;


bool is_mouse_over_btn(Btn btn, Vec2f position);

#endif
