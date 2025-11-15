#pragma once

#include <vector>
#include <unordered_map>
#include <cmath>

#include "entity.h"

struct SpatialHash {
private:

    std::unordered_map<int, std::vector<Entity*>> cell_map;

    size_t
    get_key(int32_t cell_x, int32_t cell_y);

public:
    int32_t cell_size;
    SpatialHash(int32_t size) : cell_size(size) {}

    void
    clear();

    void
    insert(Entity* entity);

    std::vector<Entity*>
    get_nearby(Entity* entity);
};
