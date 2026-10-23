#include "linear_regression.h"
#include <math.h>

bool linear_regression(const double *x, const double *y, size_t n,
                       double *slope, double *intercept) {
    if (!x || !y || !slope || !intercept || slope == intercept || n < 2) {
        return false;
    }
    double mean_x = 0.0;
    double mean_y = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(x[i]) || !isfinite(y[i])) return false;
        const double dx = x[i] - mean_x;
        const double dy = y[i] - mean_y;
        if (!isfinite(dx) || !isfinite(dy)) return false;
        mean_x += dx / (double)(i + 1);
        mean_y += dy / (double)(i + 1);
        if (!isfinite(mean_x) || !isfinite(mean_y)) return false;
    }
    double xx = 0.0, xy = 0.0;
    for (size_t i = 0; i < n; ++i) {
        const double dx = x[i] - mean_x;
        const double dy = y[i] - mean_y;
        if (!isfinite(dx) || !isfinite(dy)) return false;
        const double squared = dx * dx;
        const double product = dx * dy;
        if (!isfinite(squared) || !isfinite(product)) return false;
        xx += squared;
        xy += product;
        if (!isfinite(xx) || !isfinite(xy)) return false;
    }
    if (xx == 0.0) return false;
    const double m = xy / xx;
    const double b = mean_y - m * mean_x;
    if (!isfinite(m) || !isfinite(b)) return false;
    *slope = m;
    *intercept = b;
    return true;
}
