#include "isotonic_regression.h"
#include <math.h>

bool isotonic_regression(const double *values, size_t n, double *output) {
    if (!values || !output || n == 0 || n > 512) return false;
    for (size_t i = 0; i < n; ++i)
        if (!isfinite(values[i]) || fabs(values[i]) > 1000000.0) return false;

    double means[512];
    size_t counts[512];
    size_t blocks = 0;
    for (size_t i = 0; i < n; ++i) {
        means[blocks] = values[i];
        counts[blocks] = 1;
        ++blocks;
        while (blocks > 1 && means[blocks - 2] > means[blocks - 1]) {
            const size_t count = counts[blocks - 2] + counts[blocks - 1];
            means[blocks - 2] =
                (means[blocks - 2] * (double)counts[blocks - 2] +
                 means[blocks - 1] * (double)counts[blocks - 1]) / (double)count;
            counts[blocks - 2] = count;
            --blocks;
        }
    }
    size_t offset = 0;
    for (size_t b = 0; b < blocks; ++b)
        for (size_t j = 0; j < counts[b]; ++j)
            output[offset++] = means[b];
    return true;
}
