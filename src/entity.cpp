#include "entity.h"

Entity::Entity(float center_x, float center_y, float w, float h) {
    this->center_x = center_x;
    this->center_y = center_y;
    this->velocity_x = 1;
    this->velocity_y = 1;

    if (int32_t(this->center_x) & 1) this->velocity_x *= -1;
    if (int32_t(this->center_y) & 1) this->velocity_y *= -1;


    this->rect = SDL_FRect {
        .x = center_x - w / 2,
        .y = center_y - h / 2,
        .w = w,
        .h = h,
    };
}

void
Entity::swarm_collision(Entity& other) {
    float diff_x = this->center_x - other.center_x;
    float diff_y = this->center_y - other.center_y;

    float dist_sq = diff_x * diff_x + diff_y * diff_y;

    if (dist_sq > 0 && dist_sq < (this->rect.w * this->rect.w)) { 
        float dist = SDL_sqrtf(dist_sq);

        float dir_x = diff_x / dist;
        float dir_y = diff_y / dist;

        float push_strength = 0.1f;

        this->velocity_x += dir_x * push_strength;
        this->velocity_y += dir_y * push_strength;
    }
}
