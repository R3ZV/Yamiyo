#pragma once

#include <SDL3/SDL_render.h>
#include <vector>
#include <random>

#include "entity.h"
#include "spatial-hashing.h"

struct GameState {
public:
    const int32_t MAX_ENTITIES = 100'000;
    const float ENTITY_HEIGHT = 5;
    const float ENTITY_WIDTH = 5;

    int32_t entities_cnt;
    std::vector<Entity> entities;

    SDL_Renderer *const renderer;
    SpatialHash sph;

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<float> x_dist, y_dist;

    GameState(int32_t entities_cnt, SDL_Renderer* renderer, int32_t WIN_WIDTH, int32_t WIN_HEIGHT);

    /// It will fill the `entities` with `cnt` entities
    /// with random coordinates.
    void
    randomise_entities();

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
    check_collisions_borders(const int32_t WIN_WIDTH, const int32_t WIN_HEIGHT);

    void
    check_collisions_entities();

    void
    compute_spatial_hash();

};
