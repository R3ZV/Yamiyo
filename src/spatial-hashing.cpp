#include "spatial-hashing.h"

void
SpatialHash::clear() {
    this->cell_map.clear();
}

size_t
SpatialHash::get_key(int32_t cell_x, int cell_y) {
    size_t x = static_cast<size_t>(cell_x);
    size_t y = static_cast<size_t>(cell_y);

    return (x * 73856093) ^ (y * 19349663);
}

void
SpatialHash::insert(Entity* entity) {
    int32_t cell_x = static_cast<int32_t>(entity->center_x / this->cell_size);
    int32_t cell_y = static_cast<int32_t>(entity->center_y / this->cell_size);

    int32_t key = this->get_key(cell_x, cell_y);
    cell_map[key].push_back(entity);

    // TODO: Add entity in more buckets if it overlaps multiple cells
}

std::vector<Entity*>
SpatialHash::get_nearby(Entity* entity) {
    std::vector<Entity*> nearby;

    int32_t cell_x = static_cast<int32_t>(entity->center_x / cell_size);
    int32_t cell_y = static_cast<int32_t>(entity->center_y / cell_size);

    // Check the entity's cell AND the 8 surrounding neighbors
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            int32_t key = this->get_key(cell_x + x, cell_y + y);

            // If this cell has entities, add them to our check list
            if (cell_map.count(key)) {
                // We insert the whole vector of that cell
                auto& cellEntities = cell_map[key];
                nearby.insert(nearby.end(), cellEntities.begin(), cellEntities.end());
            }
        }
    }
    return nearby;
}
