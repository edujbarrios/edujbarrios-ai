#include "mean_variance.h"

#include <math.h>

bool mean_variance(const double *values, size_t n,
                   double *mean_out, double *variance_out) {
    if (values == NULL || n == 0 || mean_out == NULL ||
        variance_out == NULL || mean_out == variance_out) {
        return false;
    }
    double mean = 0.0;
    double m2 = 0.0;
    for (size_t i = 0; i < n; ++i) {
        const double x = values[i];
        if (!isfinite(x)) {
            return false;
        }
        const double delta = x - mean;
        if (!isfinite(delta)) {
            return false;
        }
        mean += delta / (double)(i + 1);
        if (!isfinite(mean)) {
            return false;
        }
        const double delta2 = x - mean;
        const double increment = delta * delta2;
        if (!isfinite(delta2) || !isfinite(increment)) {
            return false;
        }
        m2 += increment;
        if (!isfinite(m2)) {
            return false;
        }
    }
    const double variance = m2 / (double)n;
    if (!isfinite(variance) || variance < 0.0) {
        return false;
    }
    *mean_out = mean;
    *variance_out = variance;
    return true;
}
