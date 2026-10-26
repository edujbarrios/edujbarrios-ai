#include "softplus.h"
#include <math.h>
#include <stdint.h>

bool softplus(const double *inputs, size_t n, double *values, double *derivatives) {
    if (!inputs || !values || !derivatives || n == 0 ||
        n > SIZE_MAX / sizeof(double)) return false;
    for (size_t i = 0; i < n; ++i)
        if (!isfinite(inputs[i])) return false;
    for (size_t i = 0; i < n; ++i) {
        const double x = inputs[i];
        values[i] = fmax(x, 0.0) + log1p(exp(-fabs(x)));
        if (x >= 0.0) derivatives[i] = 1.0 / (1.0 + exp(-x));
        else {
            const double z = exp(x);
            derivatives[i] = z / (1.0 + z);
        }
    }
    return true;
}
