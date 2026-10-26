#include "minkowski_distance.h"
#include <math.h>

bool minkowski_distance(const double *a, const double *b,
                        size_t n, double p, double *result) {
    if (!a || !b || !result || n == 0 || !isfinite(p) || p < 1.0)
        return false;
    double scale = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(a[i]) || !isfinite(b[i])) return false;
        const double d = fabs(a[i] - b[i]);
        if (!isfinite(d)) return false;
        scale = fmax(scale, d);
    }
    if (scale == 0.0) { *result = 0.0; return true; }
    double sum = 0.0;
    for (size_t i = 0; i < n; ++i)
        sum += pow(fabs(a[i] - b[i]) / scale, p);
    const double answer = scale * pow(sum, 1.0 / p);
    if (!isfinite(answer)) return false;
    *result = answer;
    return true;
}
