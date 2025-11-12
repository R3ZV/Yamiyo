#include "game.h"

#include <SDL3/SDL_render.h>

GameState::GameState(int32_t entities_cnt, SDL_Renderer* renderer, int32_t WIN_WIDTH, int32_t WIN_HEIGHT) :
    entities_cnt(entities_cnt),
    renderer(renderer),
    gen(rd()),
    x_dist(ENTITY_WIDTH, static_cast<float>(WIN_WIDTH) - ENTITY_WIDTH),
    y_dist(ENTITY_HEIGHT, static_cast<float>(WIN_HEIGHT) - ENTITY_HEIGHT),
    entities(MAX_ENTITIES)
{}

void GameState::randomise_entities(int cnt) {
    for (size_t i = 0; i < cnt; i++) {
        float center_x = x_dist(gen);
        float center_y = y_dist(gen);
        entities[i] = Entity(center_x, center_y, ENTITY_WIDTH, ENTITY_HEIGHT);
    }
}

void GameState::render_entities() {
    for (auto entity : this->entities) {
        SDL_RenderRect(this->renderer, entity.rect);
    }
}
