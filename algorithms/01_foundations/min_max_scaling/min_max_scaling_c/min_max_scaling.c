#include "min_max_scaling.h"
#include <math.h>

bool min_max_scaling(const double *input, size_t n, double *output) {
    if (!input || !output || n == 0) return false;
    if (!isfinite(input[0])) return false;
    double low = input[0], high = input[0];
    for (size_t i = 1; i < n; ++i) {
        if (!isfinite(input[i])) return false;
        if (input[i] < low) low = input[i];
        if (input[i] > high) high = input[i];
    }
    const double span = high - low;
    if (!isfinite(span) || span <= 0.0) return false;
    for (size_t i = 0; i < n; ++i) {
        const double value = input[i] == low ? 0.0 :
                             input[i] == high ? 1.0 :
                             (input[i] - low) / span;
        if (!isfinite(value)) return false;
        output[i] = fmax(0.0, fmin(1.0, value));
    }
    return true;
}
