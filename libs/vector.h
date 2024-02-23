#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

typedef struct {
    float x, y;
} Vec2f;

Vec2f vec2f(float x, float y);
Vec2f addVec2f(Vec2f v1, Vec2f v2);
Vec2f subVec2f(Vec2f v1, Vec2f v2);
Vec2f mulVec2f(Vec2f v, float scalar);
float lenVec2f(Vec2f vec);
void normVec2f(Vec2f vec);

#endif
