#pragma once

#include <vector>
#include <unordered_map>
#include <cmath>

struct SpatialHash {
private:

    // (hash_key, entity_id)
    std::unordered_map<int, std::vector<size_t>> cell_map;

    size_t
    get_key(int32_t cell_x, int32_t cell_y);

public:
    int32_t cell_size;
    SpatialHash(int32_t size) : cell_size(size) {}

    void
    clear();

    void
    insert(size_t ent_id, float center_x, float center_y);

    std::vector<size_t>
    get_nearby(float center_x, float center_y);
};
