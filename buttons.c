#include "buttons.h"

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

