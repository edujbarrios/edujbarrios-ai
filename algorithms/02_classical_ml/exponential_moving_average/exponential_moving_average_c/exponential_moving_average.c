#include "exponential_moving_average.h"
#include <math.h>
#include <stddef.h>

bool exponential_moving_average(const double *values, size_t n,
                                double alpha, double *out) {
    if (n == 0 || values == NULL || out == NULL ||
        !isfinite(alpha) || alpha <= 0.0 || alpha > 1.0) {
        return false;
    }
    /* Preflight: a malformed value must not partially modify caller output. */
    double previous = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(values[i])) return false;
        previous = i == 0 ? values[0] :
                   (1.0 - alpha) * previous + alpha * values[i];
        if (!isfinite(previous)) return false;
    }
    previous = values[0];
    out[0] = previous;
    for (size_t i = 1; i < n; ++i) {
        previous = (1.0 - alpha) * previous + alpha * values[i];
        out[i] = previous;
    }
    return true;
}
