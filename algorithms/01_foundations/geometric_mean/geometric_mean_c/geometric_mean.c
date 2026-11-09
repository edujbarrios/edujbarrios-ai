#include "geometric_mean.h"
#include <math.h>

bool geometric_mean(const double *values, size_t n, double *mean_out) {
    if (!values || !mean_out || n == 0 || n > 512) return false;
    bool has_zero = false;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(values[i]) || values[i] < 0.0) return false;
        if (values[i] == 0.0) has_zero = true;
    }
    if (has_zero) {
        *mean_out = 0.0;
        return true;
    }
    double log_sum = 0.0;
    for (size_t i = 0; i < n; ++i) {
        log_sum += log(values[i]);
        if (!isfinite(log_sum)) return false;
    }
    const double result = exp(log_sum / (double)n);
    if (!isfinite(result)) return false;
    *mean_out = result;
    return true;
}
