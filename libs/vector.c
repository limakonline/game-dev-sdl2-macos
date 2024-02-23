#include <stdio.h>

#include "vector.h"

Vec2f vec2f(float x, float y) {
    Vec2f r;
    r.x = x;
    r.y = y;

    return r;
}

Vec2f addVec2f(Vec2f v1, Vec2f v2) {
    Vec2f r;
    r.x = v1.x + v2.x;
    r.y = v1.y + v2.y;

    return r;
}

Vec2f subVec2f(Vec2f v1, Vec2f v2) {
    Vec2f r;
    r.x = v1.x - v2.x;
    r.y = v1.y - v2.y;

    return r;
}

Vec2f mulVec2f(Vec2f v, float scalar) {
    Vec2f r;
    r.x = v.x * scalar;
    r.y = v.y * scalar;

    return r;
}

void normVec2f(Vec2f vec) {
    float l = lenVec2f(vec);
    if (l > 0) {
        vec = mulVec2f(vec, 1 / l);
    }
}

float lenVec2f(Vec2f vec) {
    return sqrt(vec.x * vec.x + vec.y * vec.y);
}
