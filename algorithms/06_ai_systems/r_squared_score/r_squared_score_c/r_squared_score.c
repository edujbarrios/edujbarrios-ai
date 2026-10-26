#include "r_squared_score.h"
#include <math.h>

bool r_squared_score(const double *actual, const double *predicted,
                     size_t n, double *result) {
    if (!actual || !predicted || !result || n < 2) return false;
    double target_scale = 0.0, residual_scale = 0.0;
    bool constant = true, perfect = true;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(actual[i]) || !isfinite(predicted[i])) return false;
        if (actual[i] != actual[0]) constant = false;
        if (actual[i] != predicted[i]) perfect = false;
        target_scale = fmax(target_scale, fabs(actual[i]));
        residual_scale = fmax(residual_scale, fabs(predicted[i]));
    }
    if (constant) {
        *result = perfect ? 1.0 : 0.0;
        return true;
    }
    residual_scale = fmax(residual_scale, target_scale);
    const double baseline = actual[0] / target_scale;
    double mean_offset = 0.0;
    for (size_t i = 0; i < n; ++i)
        mean_offset += (actual[i] / target_scale - baseline) / (double)n;
    double variation = 0.0, squared_error = 0.0;
    for (size_t i = 0; i < n; ++i) {
        const double dev = (actual[i] / target_scale - baseline) - mean_offset;
        const double residual =
            actual[i] / residual_scale - predicted[i] / residual_scale;
        variation += dev * dev;
        squared_error += residual * residual;
    }
    if (!isfinite(variation) || variation == 0.0 ||
        !isfinite(squared_error)) return false;
    const double magnitude = residual_scale / target_scale;
    const double ratio = squared_error / variation * magnitude * magnitude;
    const double value = 1.0 - ratio;
    if (!isfinite(value)) return false;
    *result = value;
    return true;
}
