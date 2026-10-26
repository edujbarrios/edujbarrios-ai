#include "huber_loss.h"
#include <math.h>

bool huber_loss(const double *actual, const double *predicted,
                size_t n, double delta, double *result) {
    if (!actual || !predicted || !result || n == 0 ||
        !isfinite(delta) || delta <= 0.0) return false;
    double mean = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(actual[i]) || !isfinite(predicted[i])) return false;
        const double error = fabs(actual[i] - predicted[i]);
        if (!isfinite(error)) return false;
        const double value = error <= delta ?
            0.5 * error * error : delta * (error - 0.5 * delta);
        if (!isfinite(value)) return false;
        mean += (value - mean) / (double)(i + 1);
    }
    if (!isfinite(mean)) return false;
    *result = fmax(0.0, mean);
    return true;
}
