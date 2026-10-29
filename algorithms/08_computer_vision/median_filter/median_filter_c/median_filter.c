#include "median_filter.h"
#include <math.h>

static size_t border(int value, size_t size) {
    if (value < 0) return 0;
    return (size_t)value >= size ? size - 1 : (size_t)value;
}

bool median_filter(const double *pixels, size_t height, size_t width, double *out) {
    if (!pixels || !out || height < 1 || height > 64 ||
        width < 1 || width > 64) return false;
    for (size_t i = 0; i < height * width; ++i)
        if (!isfinite(pixels[i]) || fabs(pixels[i]) > 1000000.0)
            return false;
    
    for (size_t row = 0; row < height; ++row) {
        for (size_t col = 0; col < width; ++col) {
            double window[9];
            int index = 0;
            for (int dr = -1; dr <= 1; ++dr)
                for (int dc = -1; dc <= 1; ++dc) {
                    size_t r = border((int)row + dr, height);
                    size_t c = border((int)col + dc, width);
                    window[index++] = pixels[r * width + c];
                }
            for (int i = 1; i < 9; ++i) {
                double current = window[i];
                int j = i;
                while (j > 0 && window[j - 1] > current) {
                    window[j] = window[j - 1];
                    --j;
                }
                window[j] = current;
            }
            out[row * width + col] = window[4];
        }
    }
    return true;
}
