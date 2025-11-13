#include "game.h"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL.h>

GameState::GameState(int32_t entities_cnt, SDL_Renderer* renderer, int32_t WIN_WIDTH, int32_t WIN_HEIGHT) :
    entities_cnt(entities_cnt),
    entities(size_t(MAX_ENTITIES)),
    renderer(renderer),
    gen(rd()),
    x_dist(ENTITY_WIDTH, static_cast<float>(WIN_WIDTH) - ENTITY_WIDTH),
    y_dist(ENTITY_HEIGHT, static_cast<float>(WIN_HEIGHT) - ENTITY_HEIGHT)
{}

void
GameState::randomise_entities() {
    for (size_t i = 0; i < this->entities_cnt; i++) {
        float center_x = x_dist(gen);
        float center_y = y_dist(gen);
        entities[i] = Entity(center_x, center_y, ENTITY_WIDTH, ENTITY_HEIGHT);
    }
}

void
GameState::render_entities() {
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 255, 255);
    for (size_t i = 0; i < this->entities_cnt; i++) {
        auto entity = this->entities[i];
        SDL_RenderFillRect(this->renderer, &entity.rect);
    }
}

void
GameState::update_entities() {
    for (size_t i = 0; i < this->entities_cnt; i++) {
        auto& entity = this->entities[i];
        entity.center_x += entity.velocity_x;
        entity.center_y += entity.velocity_y;
        entity.rect.x  = entity.center_x - entity.rect.w / 2;
        entity.rect.y  = entity.center_y - entity.rect.h / 2;
    };
}

void
GameState::check_collisions(const int32_t WIN_WIDTH, const int32_t WIN_HEIGHT) {
    this->check_collision_borders(WIN_WIDTH, WIN_HEIGHT);
}

void
GameState::check_collision_borders(const int32_t WIN_WIDTH, const int32_t WIN_HEIGHT) {
    for (size_t i = 0; i < this->entities_cnt; i++) {
        auto& entity = this->entities[i];
        if (entity.center_x + entity.rect.w >= WIN_WIDTH || entity.center_x - entity.rect.w <= 0) {
            entity.velocity_x *= -1;
        }

        if (entity.center_y + entity.rect.h >= WIN_HEIGHT || entity.center_y - entity.rect.h <= 0) {
            entity.velocity_y *= -1;
        }
    };
}
