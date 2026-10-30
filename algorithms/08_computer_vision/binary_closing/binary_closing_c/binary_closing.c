#include "binary_closing.h"
#include <stdlib.h>
bool binary_closing(const uint8_t *image, size_t n, size_t width, size_t height, uint8_t *output) {
    if (!image || !output || width == 0 || height == 0 ||
        width > 64 || height > 64 || n != width * height) return false;
    for (size_t i = 0; i < n; ++i) if (image[i] > 1) return false;
    uint8_t *dilated = malloc(n * sizeof(*dilated));
    if (!dilated) return false;
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            uint8_t on = 0;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    long yy = (long)y + dy, xx = (long)x + dx;
                    if (yy >= 0 && xx >= 0 && yy < (long)height && xx < (long)width &&
                        image[(size_t)yy * width + (size_t)xx] == 1) on = 1;
                }
            }
            dilated[y * width + x] = on;
        }
    }
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            uint8_t on = 1;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    long yy = (long)y + dy, xx = (long)x + dx;
                    if (yy < 0 || xx < 0 || yy >= (long)height || xx >= (long)width ||
                        dilated[(size_t)yy * width + (size_t)xx] == 0) on = 0;
                }
            }
            output[y * width + x] = on;
        }
    }
    free(dilated);
    return true;
}
