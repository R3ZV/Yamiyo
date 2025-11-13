#pragma once

#include <SDL3/SDL_render.h>
#include <vector>
#include <random>

#include "entity.h"

struct GameState {
public:
    const int32_t MAX_ENTITIES = 100'000;
    const float ENTITY_HEIGHT = 60;
    const float ENTITY_WIDTH = 80;

    int32_t entities_cnt;
    std::vector<Entity> entities;

    SDL_Renderer *const renderer;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> x_dist, y_dist;

    GameState(int32_t entities_cnt, SDL_Renderer* renderer, int32_t WIN_WIDTH, int32_t WIN_HEIGHT);

    /// It will fill the `entities` with `cnt` entities
    /// with random coordinates.
    void
    randomise_entities(size_t cnt);

    /// It will simply go through the `entities` vector and
    /// call the specific drawing function for the entity.
    void
    render_entities();

    void
    update_entities();

    void
    check_collisions(const int32_t WIN_WIDTH, const int32_t WIN_HEIGHT);

private:
    void
    check_collision_borders(const int32_t WIN_WIDTH, const int32_t WIN_HEIGHT);
};
