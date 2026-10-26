#include "chebyshev_distance.h"
#include <math.h>
#include <stddef.h>

bool chebyshev_distance(const double *left, const double *right,
                        size_t n, double *out) {
    if (out == NULL || (n > 0 && (left == NULL || right == NULL))) {
        return false;
    }
    double maximum = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(left[i]) || !isfinite(right[i])) return false;
        const double difference = left[i] - right[i];
        if (!isfinite(difference)) return false;
        const double absolute = fabs(difference);
        if (absolute > maximum) maximum = absolute;
    }
    *out = maximum;
    return true;
}
