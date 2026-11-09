#include "grayscale_contrast_stretching.h"

bool grayscale_contrast_stretching(
    const uint8_t *pixels, size_t height, size_t width, uint8_t *output) {
    if (!pixels || !output || height == 0 || height > 64 ||
        width == 0 || width > 64) return false;
    const size_t n = height * width;
    uint8_t lo = 255, hi = 0;
    for (size_t i = 0; i < n; ++i) {
        if (pixels[i] < lo) lo = pixels[i];
        if (pixels[i] > hi) hi = pixels[i];
    }
    const unsigned span = (unsigned)hi - (unsigned)lo;
    for (size_t i = 0; i < n; ++i) {
        if (span == 0) {
            output[i] = pixels[i];
        } else {
            const unsigned diff = (unsigned)pixels[i] - (unsigned)lo;
            output[i] = (uint8_t)((510u * diff + span) / (2u * span));
        }
    }
    return true;
}
