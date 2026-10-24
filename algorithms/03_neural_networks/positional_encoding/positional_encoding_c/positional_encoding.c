#include "positional_encoding.h"
#include <math.h>
#include <stdint.h>

bool positional_encoding(size_t length, size_t width, double *output) {
    if (!output || length == 0 || length > 4096 ||
        width == 0 || width > 1024 ||
        length > SIZE_MAX / width ||
        length*width > SIZE_MAX/sizeof(double)) return false;

    for (size_t p = 0; p < length; ++p) {
        for (size_t j = 0; j < width; ++j) {
            const size_t pair = j/2;
            const double exponent = (2.0*(double)pair)/(double)width;
            const double angle = (double)p / pow(10000.0, exponent);
            output[p*width+j] = j%2 == 0 ? sin(angle) : cos(angle);
        }
    }
    return true;
}
