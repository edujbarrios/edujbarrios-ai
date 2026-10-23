#include "sigmoid.h"
#include <math.h>

bool sigmoid(const double *values, size_t n, double *output) {
    if (!output || (n > 0 && !values)) return false;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(values[i])) return false;
    }
    for (size_t i = 0; i < n; ++i) {
        const double x = values[i];
        if (x >= 0.0) {
            output[i] = 1.0 / (1.0 + exp(-x));
        } else {
            const double e = exp(x);
            output[i] = e / (1.0 + e);
        }
    }
    return true;
}
