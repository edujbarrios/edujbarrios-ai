#include "integral_image.h"

bool integral_image(const uint8_t *pixels, size_t height, size_t width,
                    uint32_t *output) {
    if (!pixels || !output || height == 0 || height > 64 ||
        width == 0 || width > 64) return false;
    for (size_t row = 0; row < height; ++row) {
        uint32_t row_sum = 0;
        for (size_t col = 0; col < width; ++col) {
            const size_t i = row * width + col;
            row_sum += pixels[i];
            output[i] = row_sum + (row ? output[i - width] : 0u);
        }
    }
    return true;
}
