#include "game.h"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

GameState::GameState(int32_t entities_cnt, SDL_Renderer* renderer, int32_t WIN_WIDTH, int32_t WIN_HEIGHT) :
    // We want to perfectly divide both win_width and win_height
    sph(8),
    entities_cnt(entities_cnt),
    renderer(renderer),
    gen(rd()),
    x_dist(ENTITY_WIDTH, static_cast<float>(WIN_WIDTH) - ENTITY_WIDTH),
    y_dist(ENTITY_HEIGHT, static_cast<float>(WIN_HEIGHT) - ENTITY_HEIGHT) {

    this->get_texture("assets/mob1.png", "enemy");
}

SDL_Texture*
GameState::get_texture(const std::string& path, const std::string& name) {
    if (textures.find(name) != textures.end()) {
        return textures[name];
    }

    SDL_Texture* new_texture = IMG_LoadTexture(renderer, path.c_str());
    if (!new_texture) {
        SDL_Log("Error loading %s: %s", path.c_str(), SDL_GetError());
        return nullptr; 
    }

    textures[name] = new_texture;
    return new_texture;
}

void
GameState::cleanup_textures() {
    for (auto& pair : textures) {
        SDL_DestroyTexture(pair.second);
    }
    textures.clear();
}

void
GameState::randomise_entities() {
    for (size_t i = 0; i < this->entities_cnt; i++) {
        float center_x = x_dist(gen);
        float center_y = y_dist(gen);
        this->ents_center_x[i] = center_x;
        this->ents_center_y[i] = center_y;
        this->ents_rect[i].w = this->ENTITY_WIDTH;
        this->ents_rect[i].h = this->ENTITY_HEIGHT;
        this->ents_rect[i].x = this->ents_center_x[i] - float(this->ents_width[i]) / 2;
        this->ents_rect[i].y = this->ents_center_y[i] - float(this->ents_height[i]) / 2;

        this->ents_texture[i] = textures["enemy"];
    }
    this->compute_spatial_hash();
}

void
GameState::render_entities() {
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 255, 255);
    for (size_t i = 0; i < this->entities_cnt; i++) {
        auto entity_rect = this->ents_rect[i];
        auto entity_texture = this->ents_texture[i];
        SDL_RenderTexture(renderer, entity_texture, NULL, &entity_rect);
    }
}

void
GameState::update_entities() {
    for (size_t i = 0; i < this->entities_cnt; i++) {
        this->ents_center_x[i] += this->ents_velocity_x[i];
        this->ents_center_y[i] += this->ents_velocity_y[i];
        this->ents_rect[i].x = this->ents_center_x[i] - float(this->ents_width[i]) / 2;
        this->ents_rect[i].y = this->ents_center_y[i] - float(this->ents_height[i]) / 2;
    };
    this->compute_spatial_hash();
}

void
GameState::check_collisions(const int32_t WIN_WIDTH, const int32_t WIN_HEIGHT) {
    this->check_collisions_borders(WIN_WIDTH, WIN_HEIGHT);
    this->check_collisions_entities();
}

void
GameState::check_collisions_entities() {
    for (size_t i = 0; i < this->entities_cnt; i++) {
        auto entities_nearby = this->sph.get_nearby(this->ents_center_x[i], this->ents_center_y[i]);
        for (auto other : entities_nearby) {
            if (i == other) continue;

            swarm_collision(i, other);
        }
    }
}
void
GameState::swarm_collision(size_t self_id, size_t other_id) {
    float diff_x = this->ents_center_x[self_id] - this->ents_center_x[other_id];
    float diff_y = this->ents_center_y[self_id] - this->ents_center_y[other_id];

    float dist_sq = diff_x * diff_x + diff_y * diff_y;

    if (dist_sq > 0 && dist_sq < (this->ents_rect[self_id].w * this->ents_rect[self_id].w)) { 
        float dist = SDL_sqrtf(dist_sq);

        float dir_x = diff_x / dist;
        float dir_y = diff_y / dist;

        float push_strength = 0.1f;

        this->ents_velocity_x[self_id] += dir_x * push_strength;
        this->ents_velocity_y[self_id] += dir_y * push_strength;
    }
}

void
GameState::compute_spatial_hash() {
    this->sph.clear();
    for (size_t i = 0; i < this->entities_cnt; i++) {
        this->sph.insert(i, this->ents_center_x[i], this->ents_center_y[i]);
    };
}

void
GameState::check_collisions_borders(const int32_t WIN_WIDTH, const int32_t WIN_HEIGHT) {
    for (size_t i = 0; i < this->entities_cnt; i++) {
        if (this->ents_center_x[i] + this->ents_rect[i].w >= WIN_WIDTH ||
            this->ents_center_x[i] - this->ents_rect[i].w <= 0) {

            this->ents_velocity_x[i] *= -1;
        }

        if (this->ents_center_y[i] + this->ents_rect[i].h >= WIN_HEIGHT ||
            this->ents_center_y[i] - this->ents_rect[i].h <= 0) {
            this->ents_velocity_y[i] *= -1;
        }
    };
};
