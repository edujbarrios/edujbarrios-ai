#include "kendall_tau_b.h"
#include <math.h>

bool kendall_tau_b(const double *x, const double *y, size_t n,
                   double *output) {
    if (!x || !y || !output || n < 2 || n > 128) return false;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(x[i]) || !isfinite(y[i]) ||
            fabs(x[i]) > 1000000.0 || fabs(y[i]) > 1000000.0) return false;
    }
    double c = 0.0, d = 0.0, tx = 0.0, ty = 0.0;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            int sx = (x[i] > x[j]) - (x[i] < x[j]);
            int sy = (y[i] > y[j]) - (y[i] < y[j]);
            if (sx == 0 && sy == 0) continue;
            if (sx == 0) ++tx;
            else if (sy == 0) ++ty;
            else if (sx == sy) ++c;
            else ++d;
        }
    }
    double denominator = sqrt((c + d + tx) * (c + d + ty));
    if (denominator == 0.0) return false;
    double result = (c - d) / denominator;
    if (!isfinite(result)) return false;
    *output = fmax(-1.0, fmin(1.0, result));
    return true;
}
