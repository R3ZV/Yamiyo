#include "spatial-hashing.h"
#include <stdio.h>

void
SpatialHash::clear() {
    for (auto& cell : cells) {
        cell.clear();
    }
}

size_t
SpatialHash::get_key(int32_t cell_x, int cell_y) {
    int32_t cx = static_cast<int32_t>(cell_x / cell_size);
    if (cx < 0) cx = 0;
    if (cx >= grid_width) cx = grid_width - 1;

    int32_t cy = static_cast<int32_t>(cell_y / cell_size);
    if (cy < 0) cy = 0;
    if (cy >= grid_height) cy = grid_height - 1;

    return cx + cy * grid_width;
}

void
SpatialHash::insert(size_t ent_id, float center_x, float center_y) {
    size_t index = get_key(center_x, center_y);
    cells[index].push_back(ent_id);
}

void
SpatialHash::get_nearby(float center_x, float center_y, std::vector<size_t>& nearby) {
    int32_t cx = static_cast<int32_t>(center_x / cell_size);
    int32_t cy = static_cast<int32_t>(center_y / cell_size);

    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            int32_t neighbor_x = cx + x;
            int32_t neighbor_y = cy + y;

            if (neighbor_x >= 0 && neighbor_x < grid_width &&
                neighbor_y >= 0 && neighbor_y < grid_height) {

                int32_t index = neighbor_x + neighbor_y * grid_width;
                const auto& cell_entities = cells[index];

                nearby.insert(nearby.end(), cell_entities.begin(), cell_entities.end());
            }
        }
    }
}
