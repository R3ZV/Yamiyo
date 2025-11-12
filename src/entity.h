#pragma once

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

struct Entity {
public:
    SDL_FRect rect;
    float center_x, center_y;

    Entity(float center_x, float center_y, float w, float h);
};
