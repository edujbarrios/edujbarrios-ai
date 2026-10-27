#include "pearson_correlation.h"
#include <math.h>

bool pearson_correlation(const double *x, const double *y, size_t n,
                         double *out) {
    if (!x || !y || !out || n < 2 || n > 10000) return false;
    double mx = 0.0, my = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(x[i]) || !isfinite(y[i]) ||
            fabs(x[i]) > 1000000.0 || fabs(y[i]) > 1000000.0) return false;
        mx += x[i];
        my += y[i];
    }
    mx /= (double)n;
    my /= (double)n;
    double sxx = 0.0, syy = 0.0, sxy = 0.0;
    for (size_t i = 0; i < n; ++i) {
        const double dx = x[i] - mx, dy = y[i] - my;
        sxx += dx * dx;
        syy += dy * dy;
        sxy += dx * dy;
    }
    if (sxx <= 0.0 || syy <= 0.0) return false;
    double r = (sxy / sqrt(sxx)) / sqrt(syy);
    if (!isfinite(r)) return false;
    *out = fmax(-1.0, fmin(1.0, r));
    return true;
}
