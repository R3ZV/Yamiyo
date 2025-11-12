#include "entity.h"

Entity::Entity(float center_x, float center_y, float w, float h) {
    this->center_x = center_x;
    this->center_y = center_y;

    this->rect = SDL_FRect {
        .x = center_x - w / 2,
        .y = center_y - h / 2,
        .w = w,
        .h = h,
    };
}
