#include "grayscale_histogram_equalization.h"

bool grayscale_histogram_equalization(
    const uint8_t *pixels, size_t height, size_t width, uint8_t *output) {
    if (!pixels || !output || height == 0 || height > 64 ||
        width == 0 || width > 64) return false;

    const size_t n = height * width;
    size_t histogram[256] = {0};
    uint8_t mapping[256] = {0};
    for (size_t i = 0; i < n; ++i) ++histogram[pixels[i]];
    size_t cdf_min = 0;
    for (size_t v = 0; v < 256; ++v) {
        if (histogram[v] != 0) {
            cdf_min = histogram[v];
            break;
        }
    }
    const size_t denom = n - cdf_min;
    if (denom == 0) {
        for (size_t i = 0; i < n; ++i) output[i] = pixels[i];
        return true;
    }
    size_t cumulative = 0;
    for (size_t v = 0; v < 256; ++v) {
        cumulative += histogram[v];
        if (cumulative > cdf_min) {
            const size_t numerator = 255 * (cumulative - cdf_min);
            mapping[v] = (uint8_t)((numerator * 2 + denom) / (2 * denom));
        }
    }
    for (size_t i = 0; i < n; ++i) output[i] = mapping[pixels[i]];
    return true;
}
