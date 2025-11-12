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
GameState::randomise_entities(size_t cnt) {
    for (size_t i = 0; i < cnt; i++) {
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
        this->entities[i].center_x += this->entities[i].velocity_x;
        this->entities[i].center_y += this->entities[i].velocity_y;
        this->entities[i].rect.x  = this->entities[i].center_x - this->entities[i].rect.w / 2;
        this->entities[i].rect.y  = this->entities[i].center_y - this->entities[i].rect.h / 2;
    };
}
