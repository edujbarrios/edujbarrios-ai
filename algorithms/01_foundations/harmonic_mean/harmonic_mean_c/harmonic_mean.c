#include "harmonic_mean.h"
#include <math.h>

bool harmonic_mean(const double *values, size_t n, double *mean_out) {
    if (!values || !mean_out || n == 0 || n > 512) return false;
    double minimum = INFINITY;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(values[i]) || values[i] < 0.0) return false;
        if (values[i] < minimum) minimum = values[i];
    }
    if (minimum == 0.0) {
        *mean_out = 0.0;
        return true;
    }
    double total = 0.0;
    for (size_t i = 0; i < n; ++i) {
        total += minimum / values[i];
    }
    const double result = minimum / (total / (double)n);
    if (!isfinite(result)) return false;
    *mean_out = result;
    return true;
}
