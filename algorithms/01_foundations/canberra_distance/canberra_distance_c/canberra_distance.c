#include "canberra_distance.h"
#include <math.h>

bool canberra_distance(const double *a, const double *b,
                       size_t n, double *result) {
    if (!a || !b || !result || n == 0) return false;
    double distance = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(a[i]) || !isfinite(b[i])) return false;
        const double scale = fmax(fabs(a[i]), fabs(b[i]));
        if (scale == 0.0) continue;
        const double x = a[i] / scale, y = b[i] / scale;
        distance += fabs(x - y) / (fabs(x) + fabs(y));
        if (!isfinite(distance)) return false;
    }
    *result = distance;
    return true;
}
