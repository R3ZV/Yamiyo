#pragma once

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

struct Entity {
public:
    Entity() = default;

    SDL_FRect rect;
    float center_x, center_y;
    float velocity_x, velocity_y;

    Entity(float center_x, float center_y, float w, float h);
};
