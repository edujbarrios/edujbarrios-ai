#include "binary_hole_filling.h"

bool binary_hole_filling(
    const uint8_t *pixels, size_t height, size_t width, uint8_t *output) {
    if (!pixels || !output || height == 0 || height > 64 ||
        width == 0 || width > 64) return false;
    const size_t n = height * width;
    for (size_t i = 0; i < n; ++i) if (pixels[i] > 1) return false;

    bool seen[4096] = {false};
    size_t queue[4096], head = 0, tail = 0;
    for (size_t i = 0; i < n; ++i) {
        const size_t row = i / width, col = i % width;
        if (pixels[i] == 0 && (row == 0 || row + 1 == height ||
                               col == 0 || col + 1 == width)) {
            seen[i] = true;
            queue[tail++] = i;
        }
    }
    while (head < tail) {
        const size_t index = queue[head++];
        const size_t row = index / width, col = index % width;
        size_t neighbors[4], count = 0;
        if (row > 0) neighbors[count++] = index - width;
        if (row + 1 < height) neighbors[count++] = index + width;
        if (col > 0) neighbors[count++] = index - 1;
        if (col + 1 < width) neighbors[count++] = index + 1;
        for (size_t j = 0; j < count; ++j) {
            const size_t next = neighbors[j];
            if (pixels[next] == 0 && !seen[next]) {
                seen[next] = true;
                queue[tail++] = next;
            }
        }
    }
    for (size_t i = 0; i < n; ++i) output[i] = seen[i] ? 0 : 1;
    return true;
}
