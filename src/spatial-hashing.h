#pragma once

#include <vector>
#include <unordered_map>
#include <cmath>

struct SpatialHash {
private:

    std::vector<std::vector<size_t>> cells;

    size_t
    get_key(int32_t cell_x, int32_t cell_y);

public:
    int32_t cell_size;
    int32_t grid_width;
    int32_t grid_height;
    SpatialHash(int32_t size, int32_t win_width, int32_t win_height) : cell_size(size) {
        grid_width = win_width / size + 1;
        grid_height = win_height / size + 1;
        cells.resize(grid_width * grid_height);

        for (auto& cell : cells) {
            cell.reserve(16);
        }
    }

    void
    clear();

    void
    insert(size_t ent_id, float center_x, float center_y);

    void
    get_nearby(float center_x, float center_y, std::vector<size_t>& nearby);
};
