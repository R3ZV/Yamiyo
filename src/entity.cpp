#include "entity.h"

Entity::Entity(float center_x, float center_y, float w, float h) {
    this->center_x = center_x;
    this->center_y = center_y;
    this->velocity_x = 10;
    this->velocity_y = 10;

    this->rect = SDL_FRect {
        .x = center_x - w / 2,
        .y = center_y - h / 2,
        .w = w,
        .h = h,
    };
}

bool
Entity::collides_x(Entity& other) {
    return this->rect.x < other.rect.x + other.rect.w &&
           this->rect.x + this->rect.w > other.rect.x;
}

bool
Entity::collides_y(Entity& other) {
    return this->rect.y < other.rect.y + other.rect.h &&
           this->rect.y + this->rect.h > other.rect.y;
}
