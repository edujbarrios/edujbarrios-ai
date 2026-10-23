#include "leaky_relu.h"
#include <math.h>
bool leaky_relu(const double *values, size_t n, double alpha, double *out) {
    if (!out || (n > 0 && !values) || !isfinite(alpha) ||
        alpha <= 0.0 || alpha >= 1.0) return false;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(values[i])) return false;
    }
    for (size_t i = 0; i < n; ++i) {
        out[i] = values[i] < 0.0 ? alpha * values[i] : values[i];
    }
    return true;
}
