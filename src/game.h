#pragma once

#include <SDL3/SDL_render.h>
#include <vector>
#include <future>
#include <thread>
#include <random>

#include "spatial-hashing.h"

enum GAME_TEXTURES {
    ENEMY_TEXTURE,
};

struct GameState {
public:
    static const int32_t MAX_ENTITIES = 100'000;
    const float ENTITY_HEIGHT = 8;
    const float ENTITY_WIDTH = 8;

    float ents_center_x[MAX_ENTITIES];
    float ents_center_y[MAX_ENTITIES];
    float ents_velocity_x[MAX_ENTITIES];
    float ents_velocity_y[MAX_ENTITIES];
    int32_t ents_width[MAX_ENTITIES];
    int32_t ents_height[MAX_ENTITIES];

    SDL_FRect ents_rect[MAX_ENTITIES];
    SDL_Texture* ents_texture[MAX_ENTITIES];

    int32_t entities_cnt;
    std::vector<SDL_Texture*> textures;

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
    check_collisions_worker(size_t start_index, size_t end_index);

    void
    check_collisions_borders(const int32_t WIN_WIDTH, const int32_t WIN_HEIGHT, size_t start, size_t end);

    void
    compute_spatial_hash();

    SDL_Texture*
    get_texture(const std::string& path, const GAME_TEXTURES name);

    void
    cleanup_textures();

    void
    swarm_collision(size_t self_id, size_t other_id);
};
