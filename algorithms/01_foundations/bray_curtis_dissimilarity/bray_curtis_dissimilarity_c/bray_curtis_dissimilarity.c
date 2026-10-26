#include "bray_curtis_dissimilarity.h"
#include <math.h>

bool bray_curtis_dissimilarity(const double *a, const double *b,
                              size_t n, double *result) {
    if (!a || !b || !result || n == 0) return false;
    double scale = 0.0;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(a[i]) || !isfinite(b[i]) || a[i] < 0.0 || b[i] < 0.0)
            return false;
        scale = fmax(scale, fmax(a[i], b[i]));
    }
    if (scale == 0.0) {
        *result = 0.0;
        return true;
    }
    double numerator = 0.0, denominator = 0.0;
    for (size_t i = 0; i < n; ++i) {
        const double x = a[i] / scale, y = b[i] / scale;
        numerator += fabs(x - y);
        denominator += x + y;
    }
    const double answer = numerator / denominator;
    if (!isfinite(answer)) return false;
    *result = fmax(0.0, fmin(1.0, answer));
    return true;
}
