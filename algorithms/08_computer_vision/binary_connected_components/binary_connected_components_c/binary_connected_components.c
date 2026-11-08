#include "binary_connected_components.h"

bool binary_connected_components(const uint8_t *pixels, size_t height, size_t width,
                                 size_t *labels_out, size_t *component_count_out) {
    if (!pixels || !labels_out || !component_count_out ||
        height == 0 || height > 64 || width == 0 || width > 64) return false;
    const size_t n = height * width;
    for (size_t i = 0; i < n; ++i) if (pixels[i] > 1) return false;

    for (size_t i = 0; i < n; ++i) labels_out[i] = 0;
    size_t queue[4096];
    size_t count = 0;
    for (size_t start = 0; start < n; ++start) {
        if (pixels[start] == 0 || labels_out[start] != 0) continue;
        ++count;
        labels_out[start] = count;
        size_t head = 0, tail = 0;
        queue[tail++] = start;
        while (head < tail) {
            const size_t index = queue[head++];
            const size_t row = index / width, col = index % width;
            size_t neighbors[4], neighbors_count = 0;
            if (row > 0) neighbors[neighbors_count++] = index - width;
            if (row + 1 < height) neighbors[neighbors_count++] = index + width;
            if (col > 0) neighbors[neighbors_count++] = index - 1;
            if (col + 1 < width) neighbors[neighbors_count++] = index + 1;
            for (size_t j = 0; j < neighbors_count; ++j) {
                const size_t next = neighbors[j];
                if (pixels[next] == 1 && labels_out[next] == 0) {
                    labels_out[next] = count;
                    queue[tail++] = next;
                }
            }
        }
    }
    *component_count_out = count;
    return true;
}
