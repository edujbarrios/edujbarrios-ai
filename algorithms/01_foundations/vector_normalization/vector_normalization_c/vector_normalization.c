#include "vector_normalization.h"

#include <math.h>

bool vector_normalization(const double *left, size_t n, double *output) {
    if (left == NULL || output == NULL || n == 0) {
        return false;
    }
    double scale = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(left[i])) {
            return false;
        }
        scale = fmax(scale, fabs(left[i]));
    }
    if (scale == 0.0) {
        return false;
    }
    double squared = 0.0;
    for (size_t i = 0; i < n; ++i) {
        const double scaled = left[i] / scale;
        squared += scaled * scaled;
    }
    const double norm = sqrt(squared);
    if (!isfinite(norm) || norm == 0.0) {
        return false;
    }
    for (size_t i = 0; i < n; ++i) {
        output[i] = (left[i] / scale) / norm;
    }
    return true;
}
