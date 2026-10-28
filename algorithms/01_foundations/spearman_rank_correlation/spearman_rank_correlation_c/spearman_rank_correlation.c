#include "spearman_rank_correlation.h"
#include <math.h>

static void midranks(const double *values, size_t n, double *ranks) {
    for (size_t i = 0; i < n; ++i) {
        size_t less = 0, equal = 0;
        for (size_t j = 0; j < n; ++j) {
            if (values[j] < values[i]) ++less;
            if (values[j] == values[i]) ++equal;
        }
        ranks[i] = (double)less + ((double)equal + 1.0) / 2.0;
    }
}

bool spearman_rank_correlation(const double *x, const double *y,
                               size_t n, double *out) {
    if (!x || !y || !out || n < 2 || n > 128) return false;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(x[i]) || !isfinite(y[i]) ||
            fabs(x[i]) > 1000000.0 || fabs(y[i]) > 1000000.0) return false;
    }
    double rx[128], ry[128];
    midranks(x, n, rx);
    midranks(y, n, ry);
    const double mean = ((double)n + 1.0) / 2.0;
    double sxx = 0.0, syy = 0.0, sxy = 0.0;
    for (size_t i = 0; i < n; ++i) {
        const double dx = rx[i] - mean, dy = ry[i] - mean;
        sxx += dx * dx;
        syy += dy * dy;
        sxy += dx * dy;
    }
    if (sxx == 0.0 || syy == 0.0) return false;
    const double rho = (sxy / sqrt(sxx)) / sqrt(syy);
    if (!isfinite(rho)) return false;
    *out = fmax(-1.0, fmin(1.0, rho));
    return true;
}
