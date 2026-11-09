#include "binary_distance_transform.h"

bool binary_distance_transform(
    const uint8_t *pixels, size_t height, size_t width, uint16_t *output) {
    if (!pixels || !output || height == 0 || height > 64 ||
        width == 0 || width > 64) return false;
    const size_t n = height * width;
    for (size_t i = 0; i < n; ++i)
        if (pixels[i] > 1) return false;

    const uint16_t sentinel = (uint16_t)(height + width);
    size_t queue[4096];
    size_t head = 0, tail = 0;
    for (size_t i = 0; i < n; ++i) {
        output[i] = pixels[i] == 0 ? 0 : sentinel;
        if (pixels[i] == 0) queue[tail++] = i;
    }
    while (head < tail) {
        const size_t index = queue[head++];
        const size_t row = index / width, col = index % width;
        const uint16_t next_distance = (uint16_t)(output[index] + 1);
        size_t neighbors[4], count = 0;
        if (row > 0) neighbors[count++] = index - width;
        if (row + 1 < height) neighbors[count++] = index + width;
        if (col > 0) neighbors[count++] = index - 1;
        if (col + 1 < width) neighbors[count++] = index + 1;
        for (size_t j = 0; j < count; ++j) {
            const size_t next = neighbors[j];
            if (output[next] == sentinel) {
                output[next] = next_distance;
                queue[tail++] = next;
            }
        }
    }
    return true;
}
